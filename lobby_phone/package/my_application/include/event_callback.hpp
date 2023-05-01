#ifndef EVENT_CALLBACK_H
#define EVENT_CALLBACK_H

#include "event_driven.hpp"
#include <uv.h>

class EventDriven;

namespace EventCallback
{
	void EventQueue(uint16_t event_flag);
	void UnregisterEventCallback();
	void RegisterEventCallback(EventDriven* event);
}

#endif
