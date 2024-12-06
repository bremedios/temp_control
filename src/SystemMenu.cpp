//
// Created by Bradley Remedios on 12/5/24.
//
#include <iostream>

#include <bpl/graphics/ui/Window.h>
#include <bpl/graphics/RenderObject.h>
#include <bpl/graphics/LogicObject.h>

#include "SystemMenuLogic.h"
#include "SystemMenu.h"

bool SystemMenu::Create(bpl::graphics::RendererPtr& renderer) {
    m_name = "System Menu";

    bpl::graphics::ui::Window* uiWindowPtr = new bpl::graphics::ui::Window("views/menu_system.json", "temp_control");

    if (!uiWindowPtr->Load(renderer)) {
      std::cerr << "Failed to load System Menu Window" << std::endl;

      delete uiWindowPtr;

      return false;
    }

    bpl::graphics::RenderObjectPtr renderObject = bpl::graphics::RenderObjectPtr(uiWindowPtr);
    m_renderObjects.push_back(renderObject);

    SystemMenuLogic* systemMenuLogic = new SystemMenuLogic(renderObject);

    bpl::graphics::LogicObjectPtr logicObject = bpl::graphics::LogicObjectPtr(systemMenuLogic);

    m_logicObjects.push_back(logicObject);

    return true;
} // Create