#include "event_driven.hpp"

EventDriven::EventDriven(ConfigManager& config) : is_event_loop_running(false)
{
    /* Create Event handler */
    this->handle  = new EventHandler(config);
    
    /* Setting Event Loop */
    this->event_loop = uv_default_loop();

    /* Register Event */
    EventCallback::RegisterEventCallback(this);

    INFO_LOG("Create..");
}

void EventDriven::StartEventLoop()
{
    if (this->is_event_loop_running == false)
    {
        /* Start Event Loop */
        this->is_event_loop_running = true;
        uv_run(this->event_loop, UV_RUN_DEFAULT);
        INFO_LOG("Event loop running.");
    }
    else
    {
        ERR_LOG("An event loop is already running.");
    }
}

void EventDriven::StopEventLoop()
{
    if (this->is_event_loop_running == true)
    {
        this->is_event_loop_running = false;
        EventCallback::UnregisterEventCallback();
        uv_stop(this->event_loop);
    }
}

EventDriven::~EventDriven()
{
	this->StopEventLoop();
    delete this->handle;
    INFO_LOG("Delete..");
}

