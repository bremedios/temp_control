//
// Created by Bradley Remedios on 12/3/24.
//

#ifndef APP_H
#define APP_H

#include <bpl/graphics/Window.h>
#include <bpl/graphics/Renderer.h>
#include <bpl/graphics/EventLoop.h>
#include <bpl/graphics/RenderObject.h>
#include <bpl/controls/Input.h>

class App {
public:
    App() = default;
    ~App();

    bool Create();
    void Destroy();

    void Run();

private:
    bool                            m_initialized = false;
    bpl::graphics::WindowPtr        m_window;
    bpl::graphics::RendererPtr      m_renderer;
    bpl::graphics::RenderObjectPtr  m_renderObject;
    bpl::graphics::LogicObjectPtr   m_logicObject;
    bpl::graphics::EventLoopPtr     m_eventLoop;
    bpl::controls::InputPtr         m_input;

}; // App



#endif //APP_H