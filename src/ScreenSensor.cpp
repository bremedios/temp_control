//
// Created by Bradley Remedios on 12/5/24.
//
#include <iostream>

#include <bpl/graphics/ui/Window.h>
#include <bpl/graphics/RenderObject.h>
#include <bpl/graphics/LogicObject.h>

#include "SensorUpdater.h"
#include "ScreenSensor.h"

bool ScreenSensor::Create(bpl::graphics::RendererPtr& renderer) {
    m_name = "Temperature Sensor View";

    bpl::graphics::ui::Window* uiWindowPtr = new bpl::graphics::ui::Window("views/temperature_sensors.json", "temp_control");

    if (!uiWindowPtr->Load(renderer)) {
        std::cerr << "Failed to load Temperature Sensor Window" << std::endl;

        delete uiWindowPtr;

        return false;
    }

    bpl::graphics::RenderObjectPtr renderObject = bpl::graphics::RenderObjectPtr(uiWindowPtr);
    m_renderObjects.push_back(renderObject);

    SensorUpdater* updater = new SensorUpdater(renderObject);

    if (!updater->AddSensorClient("sensor-0", "192.168.1.215", 9999)) {
        std::cerr << "Failed to add client: 192.168.1.215:9999" << std::endl;

        return false;
    }

    if (!updater->AddSensorClient("sensor-1", "192.168.1.216", 9999)) {
        std::cerr << "Failed to add client: 192.168.1.216:9999" << std::endl;

        return false;
    }

    if (!updater->AddHeaterClient("heater-0", "192.168.1.216", 9998)) {
        std::cerr << "Failed to add client: 192.168.1.216:9998" << std::endl;

        return false;
    }

    bpl::graphics::LogicObjectPtr logicObject = bpl::graphics::LogicObjectPtr(updater);

    m_logicObjects.push_back(logicObject);

    return true;
} // Create