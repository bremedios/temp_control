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

#include "../../libbpl_controls/libbpl_controls/include/bpl/controls/Keycode.h"
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
} // Destroy

void SensorUpdater::WaitForTermination() {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Trigger termination of each client
    for (auto & client : std::views::values(m_clients)) {
        client->WaitForTermination();
    }

    m_clients.clear();
} // WaitForTermination

bool SensorUpdater::AddClient(const std::string& component, const std::string& host, const int port) {
    pinode::ClientPtr client = std::make_shared<pinode::Client>();

    if (!client->Connect(host, port)) {
        ERROR_MSG("pinode::Client::Connect(" << host << ":" << port << ") Failed");

        return false;
    }

    if (!client->Start()) {
        ERROR_MSG("pinode::Client::Start() [" << host << ":" << port << "] Failed");

        return false;
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_clients.emplace(component, client);
    }

    return true;
} // AddClient

void SensorUpdater::UpdateClient_(const std::string& component, pinode::ClientPtr client) {

    if (m_uiWindow->getJsonLoader().GetOpMap().contains(component + "-temp")) {
        bpl::graphics::draw::ops::OpPtr ptr = m_uiWindow->getJsonLoader().GetOpMap()[component + "-temp"];

        bpl::graphics::draw::ops::TextPtr textPtr = std::dynamic_pointer_cast<bpl::graphics::draw::ops::Text>(ptr);

        DEBUG_MSG("Setting : " << component + "-temp" << fmt::format(": {:.2f}", client->getTemperature()));

        textPtr->setValue(fmt::format("{:.2f}", client->getTemperature()));
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

        DEBUG_MSG("Setting : " << component + "-humidity" << fmt::format(": {:.2f}", client->getHumidity()));

        textPtr->setValue(fmt::format("{:.2f}", client->getHumidity()));
    }
} // UpdateClient_

void SensorUpdater::UpdateClients_() {
    std::lock_guard<std::mutex> lock(m_mutex);

    for (auto& client : m_clients) {
        UpdateClient_(client.first, client.second);
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

    if (now > m_nextUpdate) {
        m_nextUpdate = std::chrono::system_clock::now() + m_nextUpdateInterval;

        UpdateClients_();
    }
} // Logic