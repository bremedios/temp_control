//
// Created by bradr on 12/5/24.
//

#ifndef SYSTEMMENULOGIC_H
#define SYSTEMMENULOGIC_H

#include <bpl/graphics/Renderer.h>
#include <bpl/graphics/LogicObject.h>
#include <bpl/graphics/RenderObject.h>
#include <bpl/graphics/ui/Window.h>

#include <bpl/controls/Input.h>

class SystemMenuLogic : public bpl::graphics::LogicObject{
public:
    SystemMenuLogic(bpl::graphics::RenderObjectPtr& renderObject);
    ~SystemMenuLogic() override = default;

    void Logic(bpl::graphics::RendererPtr& renderer, bpl::controls::InputPtr& input) override;
private:
    enum SystemMenuState : int {
        SYSTEM_MENU_STATE_START=0,
        SYSTEM_MENU_STATE_RESUME,
        SYSTEM_MENU_STATE_DISPLAY_SETTINGS,
        SYSTEM_MENU_STATE_PROGRAM_SETTINGS,
        SYSTEM_MENU_STATE_EXIT,
        SYSTEM_MENU_STATE_END
    };

    SystemMenuState                 m_state =SYSTEM_MENU_STATE_RESUME;
    bpl::graphics::RenderObjectPtr  m_renderObject;
    bpl::graphics::ui::Window*      m_uiWindow      = nullptr;
}; // SystemMenuLogic



#endif //SYSTEMMENULOGIC_H