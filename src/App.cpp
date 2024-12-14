//
// Created by bradr on 12/3/24.
//
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <bpl/sys/Hardware.h>

#include <bpl/graphics/FontCache.h>
#include <bpl/graphics/ui/Window.h>
#include <bpl/controls/Input.h>

#include "App.h"
#include "ScreenSensor.h"
#include <bpl/graphics/screens/ScreenStateStack.h>

#include "SystemMenu.h"

#define OVERLAY_WIDTH 720
#define OVERLAY_HEIGHT 720

App::~App() {
} //~App

bool App::Create() {
    bpl::sys::Hardware hw;

    if (SDL_Init(SDL_INIT_VIDEO)< 0) {
        std::cerr << "SDL Could not initialize.  SDL ERROR: " << SDL_GetError() << std::endl;

        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "SDL TTF Could not initialize.  SDL ERROR: " << SDL_GetError() << std::endl;

        SDL_Quit();
        return false;
    }

    m_input = std::make_shared<bpl::controls::Input>();

    m_input->Create();
    m_input->ScanJoysticks();

    m_window = bpl::graphics::Window::CreateInstance("D&D Beyond Overlay", hw.getDisplayWidth(), hw.getDisplayHeight(), hw.getFullscreen());

    if (!m_window->isValid()) {
        std::cerr << "Failed to create window" << std::endl;

        TTF_Quit();
        SDL_Quit();
        return false;
    }

    m_renderer = bpl::graphics::Renderer::CreateInstance(m_window);

    if (!m_renderer->isValid()) {
        std::cerr << "Failed to create renderer" << std::endl;

        TTF_Quit();
        SDL_Quit();
        return false;
    }

    bpl::graphics::ui::Window* uiWindowPtr = new bpl::graphics::ui::Window("views/temperature_sensors.json", "temp_control");

    if (!uiWindowPtr->Load(m_renderer)) {
        std::cerr << "Failed to load UI Window" << std::endl;

        delete uiWindowPtr;

        TTF_Quit();
        SDL_Quit();

        return false;
    }

    // Initialize event loop
    m_eventLoop= std::make_shared<bpl::graphics::EventLoop>();
    m_eventLoop->setFramerate(30);
    m_eventLoop->setRenderer(m_renderer);

    // Create our screen stack which will manage all of our screens
    bpl::graphics::screens::ScreenStateStack::getInstance()->setEventLoop(m_eventLoop);

    ScreenSensor* screenSensor = new ScreenSensor();
    bpl::graphics::screens::ScreenObjectPtr sensorPtr = bpl::graphics::screens::ScreenObjectPtr(screenSensor);

    // Create our screen sensor
    if (!screenSensor->Create(m_renderer)) {
        std::cerr << "Failed to create ScreenSensor" << std::endl;

        TTF_Quit();
        SDL_Quit();
        return false;
    }

    SystemMenu* systemMenu = new SystemMenu();
    bpl::graphics::screens::ScreenObjectPtr systemMenuPtr = bpl::graphics::screens::ScreenObjectPtr(systemMenu);

    // Create our screen sensor
    if (!systemMenu->Create(m_renderer)) {
        std::cerr << "Failed to create SystemMenu" << std::endl;

        TTF_Quit();
        SDL_Quit();
        return false;
    }

    bpl::graphics::screens::ScreenStateStack::getInstance()->AddScreen(sensorPtr);
    bpl::graphics::screens::ScreenStateStack::getInstance()->AddScreen(systemMenuPtr);

    bpl::graphics::screens::ScreenStateStack::getInstance()->Push("Temperature Sensor View");

    m_initialized = true;

    return true;
} // Create

void App::Destroy() {
    if (!m_initialized) {
        std::cout << "Not Initialized, exiting Destroy" << std::endl;
        return;
    }

    bpl::graphics::screens::ScreenStateStack::getInstance()->Clear();

    if (nullptr != m_logicObject.get()) {
        m_logicObject->Destroy();
        m_logicObject->WaitForTermination();
    }

    if (nullptr != m_eventLoop.get()) {
        m_eventLoop->Clear();
        m_eventLoop->Terminate();
        m_eventLoop.reset();
    }

    if (nullptr != m_renderObject.get()) {
        m_renderObject.reset();
    }

    if (nullptr != m_input.get()) {
        m_input->Destroy();
        m_input.reset();
    }

    if (nullptr != m_renderer.get()) {
        m_renderer->Destroy();
        m_renderer.reset();
    }

    if (nullptr != m_window.get()) {
        m_window.reset();
    }

    bpl::graphics::FontCache::getInstance()->Clear();

    TTF_Quit();
    SDL_Quit();

    m_initialized = false;
} // Destroy

void App::Run() {
    m_eventLoop->Run();
} // Run