//
// Created by Bradley Remedios on 11/20/24.
//
#include <iostream>
#include <filesystem>

#include <bpl/net/Http.h>
#include <bpl/graphics/draw/ops/Fill.h>
#include <bpl/graphics/draw/ops/Image.h>
#include <bpl/graphics/draw/ops/Text.h>
#include <bpl/graphics/screens/ScreenStateStack.h>

#include <fmt/format.h>

#include <bpl/controls/Keycode.h>
#include "Debug.h"
#include "SensorUpdater.h"


SensorUpdater::SensorUpdater(bpl::graphics::RenderObjectPtr& renderObject)
    : m_renderObject(renderObject) {

    m_uiWindow = dynamic_cast<bpl::graphics::ui::Window*>(renderObject.get());

}

SensorUpdater::~SensorUpdater() {
    Destroy();
    WaitForTermination();
}

void SensorUpdater::Destroy() {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Trigger termination of each client
    for (auto & client : std::views::values(m_clients)) {
        client->Terminate();
    }
    m_clients.clear();
} // Destroy

void SensorUpdater::WaitForTermination() {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Trigger termination of each client
    for (auto & client : std::views::values(m_clients)) {
        client->WaitForTermination();
    }

    m_clients.clear();
} // WaitForTermination

bool SensorUpdater::AddHeaterClient(const std::string& component, const std::string& host, const int port) {
    pinode::ClientPtr client = std::make_shared<pinode::Client>();

    if (!client->Connect(host, port)) {
        ERROR_MSG("pinode::Client::Connect(" << host << ":" << port << ") Failed");

        return false;
    }

    if (!client->Start()) {
        ERROR_MSG("pinode::Client::Start() [" << host << ":" << port << "] Failed");

        return false;
    }

    client->EnableHeater(true);

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_clients.emplace(component, client);
    }

    return true;
} // AddHeaterClient

bool SensorUpdater::AddSensorClient(const std::string& component, const std::string& host, const int port) {
    pinode::ClientPtr client = std::make_shared<pinode::Client>();

    if (!client->Connect(host, port)) {
        ERROR_MSG("pinode::Client::Connect(" << host << ":" << port << ") Failed");

        return false;
    }

    if (!client->Start()) {
        ERROR_MSG("pinode::Client::Start() [" << host << ":" << port << "] Failed");

        return false;
    }

    client->EnableSensor(true);

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_clients.emplace(component, client);
    }

    return true;
} // AddSensorClient

void SensorUpdater::UpdateClient_(const std::string& component, pinode::ClientPtr client) {

    if (m_uiWindow->getJsonLoader().GetOpMap().contains(component + "-temp")) {
        bpl::graphics::draw::ops::OpPtr ptr = m_uiWindow->getJsonLoader().GetOpMap()[component + "-temp"];

        bpl::graphics::draw::ops::TextPtr textPtr = std::dynamic_pointer_cast<bpl::graphics::draw::ops::Text>(ptr);

        if (client->hasTemperature()) {
            DEBUG_MSG("Setting : " << component + "-temp" << fmt::format(": {:.2f}", client->getTemperature()));

            textPtr->setValue(fmt::format("{:.2f}", client->getTemperature()));
        }
        else {
            textPtr->setValue("----");
        }
    }

    if (m_uiWindow->getJsonLoader().GetOpMap().contains(component + "-title")) {
        bpl::graphics::draw::ops::OpPtr ptr = m_uiWindow->getJsonLoader().GetOpMap()[component + "-title"];

        bpl::graphics::draw::ops::TextPtr textPtr = std::dynamic_pointer_cast<bpl::graphics::draw::ops::Text>(ptr);

        DEBUG_MSG("Setting : " << component + "-title" << fmt::format(": {}", client->getName()));

        textPtr->setValue(fmt::format("{}", client->getName()));
    }

    if (m_uiWindow->getJsonLoader().GetOpMap().contains(component + "-humidity")) {
        bpl::graphics::draw::ops::OpPtr ptr = m_uiWindow->getJsonLoader().GetOpMap()[component + "-humidity"];

        bpl::graphics::draw::ops::TextPtr textPtr = std::dynamic_pointer_cast<bpl::graphics::draw::ops::Text>(ptr);

        if (client->hasHumidity()) {
            DEBUG_MSG("Setting : " << component + "-humidity" << fmt::format(": {:.2f}", client->getHumidity()));

            textPtr->setValue(fmt::format("{:.1f} %", client->getHumidity()));
        }
        else {
            textPtr->setValue("----");
        }
    }
} // UpdateClient_

void SensorUpdater::UpdateHeaterStatus_(pinode::ClientPtr client) {
    auto heaterStatus = client->getHeaterStatus();

    if (nullptr == heaterStatus.get()) {
        ERROR_MSG("Failed to get heater status");

        if (m_uiWindow->getJsonLoader().GetOpMap().contains("heater-temp-prog")) {
            bpl::graphics::draw::ops::OpPtr ptr = m_uiWindow->getJsonLoader().GetOpMap()["heater-temp-prog"];

            bpl::graphics::draw::ops::TextPtr textPtr = std::dynamic_pointer_cast<bpl::graphics::draw::ops::Text>(ptr);

            textPtr->setValue("----");
            textPtr->setVisible(true);
        }

        return;
    }

    // Check to see if we no longer need to show a pending override
    if (m_overridePending) {
        if (heaterStatus->getTemperature() == m_pendingTemperature) {
            m_overridePending = false;
        }
    }

    // temperature override driven by heater
    if (m_uiWindow->getJsonLoader().GetOpMap().contains("heater-temp")) {
        bpl::graphics::draw::ops::OpPtr ptr = m_uiWindow->getJsonLoader().GetOpMap()["heater-temp"];

        bpl::graphics::draw::ops::TextPtr textPtr = std::dynamic_pointer_cast<bpl::graphics::draw::ops::Text>(ptr);

        DEBUG_MSG("Setting : " << "heater-temp" << fmt::format(": {:.1f}", heaterStatus->getTemperature()));

        textPtr->setValue(fmt::format("{:.2f}", heaterStatus->getTemperature()));

        textPtr->setVisible(!m_overridePending && heaterStatus->isOverridden());
    }

    // temperature of program
    if (m_uiWindow->getJsonLoader().GetOpMap().contains("heater-temp-prog")) {
        bpl::graphics::draw::ops::OpPtr ptr = m_uiWindow->getJsonLoader().GetOpMap()["heater-temp-prog"];

        bpl::graphics::draw::ops::TextPtr textPtr = std::dynamic_pointer_cast<bpl::graphics::draw::ops::Text>(ptr);

        DEBUG_MSG("Setting : " << "heater-temp-prog" << fmt::format(": {:.1f}", heaterStatus->getProgramTemperature()));

        textPtr->setValue(fmt::format("{:.2f}", heaterStatus->getProgramTemperature()));
        textPtr->setVisible(!m_overridePending && !heaterStatus->isOverridden());
    }

    // Temperature override has not been set yet
    if (m_uiWindow->getJsonLoader().GetOpMap().contains("heater-temp-pending")) {
        bpl::graphics::draw::ops::OpPtr ptr = m_uiWindow->getJsonLoader().GetOpMap()["heater-temp-pending"];

        bpl::graphics::draw::ops::TextPtr textPtr = std::dynamic_pointer_cast<bpl::graphics::draw::ops::Text>(ptr);

        DEBUG_MSG("Setting : " << "heater-temp-pending" << fmt::format(": {:.1f}", heaterStatus->getTemperature()));

        textPtr->setValue(fmt::format("{:.2f}", m_pendingTemperature));
        textPtr->setVisible(m_overridePending);
    }

    if (m_uiWindow->getJsonLoader().GetOpMap().contains("heater-state")) {
        bpl::graphics::draw::ops::OpPtr ptr = m_uiWindow->getJsonLoader().GetOpMap()["heater-state"];

        bpl::graphics::draw::ops::TextPtr textPtr = std::dynamic_pointer_cast<bpl::graphics::draw::ops::Text>(ptr);

        textPtr->setVisible(heaterStatus->isOn());
    }

    if (m_uiWindow->getJsonLoader().GetOpMap().contains("program-state-program")) {
        bpl::graphics::draw::ops::OpPtr ptr = m_uiWindow->getJsonLoader().GetOpMap()["program-state-program"];

        bpl::graphics::draw::ops::TextPtr textPtr = std::dynamic_pointer_cast<bpl::graphics::draw::ops::Text>(ptr);

        textPtr->setVisible(!heaterStatus->isOverridden());
    }

    if (m_uiWindow->getJsonLoader().GetOpMap().contains("program-state-override")) {
        bpl::graphics::draw::ops::OpPtr ptr = m_uiWindow->getJsonLoader().GetOpMap()["program-state-override"];

        bpl::graphics::draw::ops::TextPtr textPtr = std::dynamic_pointer_cast<bpl::graphics::draw::ops::Text>(ptr);

        textPtr->setVisible(heaterStatus->isOverridden());
    }
} // UpdateHeaterStatus_

void SensorUpdater::UpdateClients_() {
    std::lock_guard<std::mutex> lock(m_mutex);

    int heaterCount = 0;

    for (auto& client : m_clients) {

        if (client.second->hasHeaterStatus()) {
            if (heaterCount > 0) {
                ERROR_MSG("Too many heaters registered");
            }
            UpdateHeaterStatus_(client.second);

            heaterCount++;
        }
        if (client.second->hasTemperature() || client.second->hasHumidity()) {
            UpdateClient_(client.first, client.second);
        }
    }
} // void SensorUpdater::UpdateClients_() {

void SensorUpdater::Logic(bpl::graphics::RendererPtr& renderer, bpl::controls::InputPtr& input) {
    auto now = std::chrono::system_clock::now();

    if (input->KeyDown(bpl::controls::KeyCode::INPUT_START) && input->KeyDown(bpl::controls::KeyCode::INPUT_SELECT)) {
        DEBUG_MSG("Start and Select Down");
        bpl::graphics::screens::ScreenStateStack::getInstance()->Push("QUIT PROGRAM");
    }
    else if (input->KeyPressed(bpl::controls::KeyCode::INPUT_START)) {
        DEBUG_MSG("Pushing System Menu to screen stack")
        bpl::graphics::screens::ScreenStateStack::getInstance()->Push("System Menu");
    }
    else if (input->KeyPressed(bpl::controls::KeyCode::INPUT_UP)) {
        DEBUG_MSG("Temperature Up");

        for (auto& client : std::views::values(m_clients)) {
            if (client->hasHeaterStatus()) {
                if (!m_overridePending) {
                    m_overridePending = true;
                    m_pendingTemperature = client->getHeaterStatus()->getTemperature();
                }

                m_pendingTemperature = m_pendingTemperature + 0.25;

                client->SendTemperatureOverride(m_pendingTemperature);
            }
        }
    }
    else if (input->KeyPressed(bpl::controls::KeyCode::INPUT_DOWN)) {
        DEBUG_MSG("Temperature Up");

        for (auto& client : std::views::values(m_clients)) {
            if (client->hasHeaterStatus()) {
                if (!m_overridePending) {
                    m_overridePending = true;
                    m_pendingTemperature = client->getHeaterStatus()->getTemperature();
                }

                m_pendingTemperature = m_pendingTemperature - 0.25;

                client->SendTemperatureOverride(m_pendingTemperature);
            }
        }
    }
    else if (input->KeyPressed(bpl::controls::KeyCode::INPUT_SELECT)) {
        DEBUG_MSG("Temperature Up");

        for (auto& client : std::views::values(m_clients)) {
            if (client->hasHeaterStatus()) {
                client->ClearTemperatureOverride();
            }
        }
    }

    if (now > m_nextUpdate) {
        m_nextUpdate = std::chrono::system_clock::now() + m_nextUpdateInterval;

        UpdateClients_();
    }
} // Logic