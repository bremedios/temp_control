//
// Created by Bradley Remedios on 12/5/24.
//

#ifndef SCREENSENSOR_H
#define SCREENSENSOR_H

#include <bpl/graphics/screens/ScreenObject.h>

class ScreenSensor : public bpl::graphics::screens::ScreenObject{
public:
    ScreenSensor() = default;
    ~ScreenSensor() override = default;

    bool Create(bpl::graphics::RendererPtr& renderer);

}; // ScreenSensor

typedef std::shared_ptr<ScreenSensor> ScreenSensorPtr;



#endif //SCREENSENSOR_H