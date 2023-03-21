#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <cstdint>
#include <atomic>

#include "oled.hpp"
#include "keyPad.hpp"
#include "systemManager.hpp"
#include "buzzer.hpp"
#include "mqtt_iface.hpp"

class EventHandler
{
public:
    EventHandler();
    void KeypadEventHandler();
    void RenewScreenTimeHandler();
    ~EventHandler();

private:
    Buzzer* buzzer;
    KeyPad* key_pad;
    Oled* oled;
    SystemManager* sys_mgr;
    MqttIface* mqtt_iface;    
    std::atomic<unsigned char> pw_input_count;
#if (0)
    std::atomic<uint64_t> curr_display_lock_tm;
    std::atomic<uint64_t> prev_display_lock_tm;
    std::atomic<uint64_t> display_lock_tm;
#else
    std::atomic<unsigned long> curr_display_lock_tm;
    std::atomic<unsigned long> prev_display_lock_tm;
    std::atomic<unsigned long> display_lock_tm;
#endif
    std::atomic<bool> is_init_display;
    
    void ClearEvent();
    void OpenDoorEvent();
    void DisplayInitialScreen();
    void WritePasswordOnScreen(const char* data);
    
    #define  PASSWORD   'A'
    #define  CALL       'B'
    #define  CLEAR      'C'
    #define  NULL_DATA  0
};
#endif
