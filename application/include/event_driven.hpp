#ifndef EVENT_DRIVEN_H
#define EVENT_DRIVEN_H

#include <ev.h>

#include "thread_pool.hpp"
#include "event_handler.hpp"

class EventDriven
{
public:
    struct ev_timer device_timer;
    struct ev_timer network_timer;
    
    EventDriven(size_t num_threads);
    void StartEventLoop();
    void StopEventLoop();
    void StartTimer();
    void StopTimer();
    void DeviceEventListener(uint16_t event_flag);
    void DeviceTimerHandler(struct ev_loop *loop, ev_timer *w, int revents);
    void NetworkTimerHandler(struct ev_loop *loop, ev_timer *w, int revents);
    void NetworkEventListener(uint16_t event_flag);
    ~EventDriven();

private:
    ThreadPool* pool;
    EventHandler* handle;
    
    struct ev_loop* event_loop;
    
    bool is_event_loop_running = false;
    static constexpr uint16_t KEYPAD_EVENT      = 0u;
    static constexpr uint16_t TIME_UPDATE_EVENT = 1u;
    static constexpr uint16_t MAX_EVENT         = 2u;
    
    static constexpr uint16_t BROKER_RETRY_CON_EVENT   = 0u;
    static constexpr uint16_t MAX_NETWORK_EVENT        = 1u;

    static constexpr uint16_t TIME_UPDATE_EVENT_PERIOD      = 10u;   // 1s
    static constexpr uint16_t KEYPAD_EVENT_PERIOD           = 1u;   // 0.1s 
    static constexpr uint16_t BROKER_RETRY_CON_EVENT_PERIOD = 30u;  // 3s
    
};
#endif
