#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <cstdint>
#include <atomic>

#include "oled.hpp"
#include "keyPad.hpp"
#include "systemManager.hpp"
#include "buzzer.hpp"
#include "mqtt_iface.hpp"
#include "video.hpp"
#include "yaml-cpp/yaml.h"

class EventHandler : public Utils
{
public:
    EventHandler(const YAML::Node &config);
    void KeypadEventHandler();
    void RenewScreenTimeHandler();
    void RetryConBrokerHandler();
    ~EventHandler();

private:
    Buzzer* buzzer;
    KeyPad* key_pad;
    Oled* oled;
    SystemManager* sys_mgr;
    MqttIface* mqtt_iface;   
    Video* video; 
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
    
    #define  PASSWORD   'A'
    #define  CALL       'B'
    #define  CLEAR      'C'
    #define  NULL_DATA  0
};
#endif
