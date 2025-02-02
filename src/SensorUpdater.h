//
// Created by Bradley Remedios on 11/20/24.
//

#ifndef SENSOR_UPDATER_H
#define SENSOR_UPDATER_H

#include <chrono>
#include <mutex>

#include <bpl/graphics/LogicObject.h>
#include <bpl/graphics/ui/Window.h>

#include <pinode/Client.h>

class SensorUpdater  : public bpl::graphics::LogicObject{
public:
    SensorUpdater(bpl::graphics::RenderObjectPtr& renderObject);
    ~SensorUpdater() override;

    void Destroy() override;
    void WaitForTermination() override;

    bool AddHeaterClient(const std::string& component, const std::string& host, const int port);
    bool AddSensorClient(const std::string& component, const std::string& host, const int port);

    void Logic(bpl::graphics::RendererPtr& renderer, bpl::controls::InputPtr& input) override;
private:
    void UpdateClient_(const std::string& component, pinode::ClientPtr client);
    void UpdateHeaterStatus_(pinode::ClientPtr client);
    void UpdateClients_();

    std::chrono::time_point<std::chrono::system_clock>  m_nextUpdate = std::chrono::system_clock::now();
    std::chrono::milliseconds                           m_nextUpdateInterval = std::chrono::milliseconds(1000);

    bpl::graphics::RenderObjectPtr  m_renderObject;
    bpl::graphics::ui::Window*      m_uiWindow      = nullptr;
    bool                            m_firstRun      = true;
    std::mutex                      m_mutex;
    float                           m_pendingTemperature = 0.0f;
    bool                            m_overridePending=false;

    std::map<std::string, pinode::ClientPtr> m_clients;
}; // SensorUpdater


#endif //SENSOR_UPDATER_H