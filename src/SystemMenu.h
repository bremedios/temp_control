//
// Created by Bradley Remedios on 12/5/24.
//

#ifndef SYSTEM_MENU_H
#define SYSTEM_MENU_H

#include <bpl/graphics/screens/ScreenObject.h>

class SystemMenu : public bpl::graphics::screens::ScreenObject {
public:
    SystemMenu() = default;
    ~SystemMenu() override = default;

    bool Create(bpl::graphics::RendererPtr& renderer);


}; // SystemMenu

typedef std::shared_ptr<SystemMenu> SystemMenuPtr;

#endif //SYSTEM_MENU_H