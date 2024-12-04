//
// Created by bradr on 12/3/24.
//
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <bpl/graphics/FontCache.h>
#include <bpl/graphics/ui/Window.h>

#include "App.h"
//#include "SensorUpdater.h"

#define OVERLAY_WIDTH 720
#define OVERLAY_HEIGHT 720

App::~App() {
} //~App

bool App::Create() {
    if (SDL_Init(SDL_INIT_VIDEO)< 0) {
        std::cerr << "SDL Could not initialize.  SDL ERROR: " << SDL_GetError() << std::endl;

        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "SDL TTF Could not initialize.  SDL ERROR: " << SDL_GetError() << std::endl;

        SDL_Quit();
        return false;
    }

    m_window = bpl::graphics::Window::CreateInstance("D&D Beyond Overlay", OVERLAY_WIDTH, OVERLAY_HEIGHT, false);

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

    bpl::graphics::ui::Window* uiWindowPtr = new bpl::graphics::ui::Window("views/temperature_sensors.json");

    if (!uiWindowPtr->Load(m_renderer)) {
        std::cerr << "Failed to load UI Window" << std::endl;

        delete uiWindowPtr;

        TTF_Quit();
        SDL_Quit();

        return false;
    }

    m_renderObject = bpl::graphics::RenderObjectPtr(uiWindowPtr);

    m_eventLoop= std::make_shared<bpl::graphics::EventLoop>();

    //SensorUdpater* updater = new SensorUdpater(m_renderObject);

    //m_logicObject = bpl::graphics::LogicObjectPtr(updater);

    m_eventLoop->setFramerate(30);
    m_eventLoop->setRenderer(m_renderer);
    m_eventLoop->addRenderObject(m_renderObject);
    //m_eventLoop->addLogicObject(m_logicObject);

    return true;
} // Create

void App::Destroy() {
    if (!m_initialized) {
        return;
    }

    bpl::graphics::FontCache::getInstance()->Clear();

    TTF_Quit();
    SDL_Quit();

    m_initialized = false;
} // Destroy

void App::Run() {
    m_eventLoop->Run();
} // Run