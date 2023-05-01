#ifndef EVENT_DRIVEN_H
#define EVENT_DRIVEN_H

#include <uv.h>

#include "event_handler.hpp"
#include "config_manager.hpp"
#include "utils.hpp"
#include "event_callback.hpp"

class EventDriven : public Utils
{
public:
    uv_loop_t* event_loop;
    EventHandler* handle;

    EventDriven(ConfigManager& config);
    void StartEventLoop();
    void StopEventLoop();
    ~EventDriven();

private:
    bool is_event_loop_running;
};
#endif
