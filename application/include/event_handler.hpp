#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <cstdint>
#include <atomic>
#include <memory>

#include "oled.hpp"
#include "keyPad.hpp"
#include "systemManager.hpp"
#include "buzzer.hpp"
#include "mqtt_iface.hpp"
#include "image_sender.hpp"
#include "config_manager.hpp"
#include "utils.hpp"

class EventHandler : public Utils
{
public:
    EventHandler(ConfigManager& config);
    void KeypadEventHandler();
    void RenewScreenTimeHandler();
    void MqttYieldEventHandler();
    ~EventHandler();

private:
    Buzzer* buzzer;
    KeyPad* key_pad;
    Oled* oled;
    SystemManager* sys_mgr;
    MqttIface* mqtt_iface;   
    ImageSender* image_sender;
    ConfigManager* config;

    std::atomic<unsigned char> pw_input_count;
    std::atomic<unsigned long> prev_display_lock_tm;
    std::atomic<unsigned long> display_lock_tm;
    std::atomic<bool> is_init_display;
    
    void IntercomEvent();
    void ClearEvent();
    void SuccessPwEvent();
    void FailPwEvent();
    void AllowAccessEvent();
    void DenyAccessEvent();
    void OpenDoor();
    void LockDisplay(uint64_t lock_tm);
    void InitDisplay();
    void WritePasswordOnDisplay(const char* data);
    const char* CreateOpenDoorMsg();
};
#endif
