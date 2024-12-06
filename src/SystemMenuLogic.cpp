//
// Created by Bradley Remedios on 12/5/24.
//

#include "SystemMenuLogic.h"
#include <bpl/graphics/screens/ScreenStateStack.h>

SystemMenuLogic::SystemMenuLogic(bpl::graphics::RenderObjectPtr& renderObject)
    : m_renderObject(renderObject) {

    m_uiWindow = dynamic_cast<bpl::graphics::ui::Window*>(renderObject.get());
}


void SystemMenuLogic::Logic(bpl::graphics::RendererPtr& renderer, bpl::controls::InputPtr& input) {

    // This is cancel
    if (input->KeyPressed(bpl::controls::KeyCode::INPUT_ESCAPE) || input->KeyPressed(bpl::controls::KeyCode::INPUT_LEFT)) {
        bpl::graphics::screens::ScreenStateStack::getInstance()->Pop();
    }

    // Handle selection of menu item
    if (input->KeyPressed(bpl::controls::KeyCode::INPUT_ENTER) || input->KeyPressed(bpl::controls::KeyCode::INPUT_RIGHT)) {
        switch (m_state) {
        case SYSTEM_MENU_STATE_RESUME:
            bpl::graphics::screens::ScreenStateStack::getInstance()->Pop();
            break;
        case SYSTEM_MENU_STATE_DISPLAY_SETTINGS:
            bpl::graphics::screens::ScreenStateStack::getInstance()->Pop();
            break;
        case SYSTEM_MENU_STATE_PROGRAM_SETTINGS:
            bpl::graphics::screens::ScreenStateStack::getInstance()->Pop();
            break;
        case SYSTEM_MENU_STATE_EXIT:
            bpl::graphics::screens::ScreenStateStack::getInstance()->Push("QUIT PROGRAM");
            break;
        }
    }

    if (input->KeyPressed(bpl::controls::KeyCode::INPUT_UP)) {
        m_state = static_cast<SystemMenuState>(std::max(SYSTEM_MENU_STATE_START+1, m_state - 1));
    }
    if (input->KeyPressed(bpl::controls::KeyCode::INPUT_DOWN)) {
        m_state = static_cast<SystemMenuState>(std::min(SYSTEM_MENU_STATE_END-1, m_state + 1));
    }

    switch (m_state) {
    case SYSTEM_MENU_STATE_RESUME:
        m_uiWindow->getJsonLoader().GetOpMap()["resume-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["resume-not-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["display-settings-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["display-settings-not-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["program-settings-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["program-settings-not-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["exit-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["exit-not-selected"]->setVisible(true);
        break;
    case SYSTEM_MENU_STATE_DISPLAY_SETTINGS:
        m_uiWindow->getJsonLoader().GetOpMap()["resume-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["resume-not-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["display-settings-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["display-settings-not-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["program-settings-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["program-settings-not-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["exit-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["exit-not-selected"]->setVisible(true);
        break;
    case SYSTEM_MENU_STATE_PROGRAM_SETTINGS:
        m_uiWindow->getJsonLoader().GetOpMap()["resume-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["resume-not-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["display-settings-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["display-settings-not-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["program-settings-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["program-settings-not-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["exit-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["exit-not-selected"]->setVisible(true);
        break;
    case SYSTEM_MENU_STATE_EXIT:
        m_uiWindow->getJsonLoader().GetOpMap()["resume-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["resume-not-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["display-settings-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["display-settings-not-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["program-settings-selected"]->setVisible(false);
        m_uiWindow->getJsonLoader().GetOpMap()["program-settings-not-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["exit-selected"]->setVisible(true);
        m_uiWindow->getJsonLoader().GetOpMap()["exit-not-selected"]->setVisible(false);
        break;
    }
} // Logic