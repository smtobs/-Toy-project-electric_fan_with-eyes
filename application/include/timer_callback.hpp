#ifndef TIMER_CALLBACK_H
#define TIMER_CALLBACK_H
#include <ev.h>
#include "event_driven.hpp"

static void CallBackTimerDeivce(struct ev_loop *loop, ev_timer *w, int revents);

namespace TimerCallBack
{
	static EventDriven* g_event;
	void RegisterTimer(EventDriven* event)
	{
		g_event = event;
		ev_timer_init(&g_event->device_timer, CallBackTimerDeivce, 1.0, 0.1);
	}
}

static void CallBackTimerDeivce(struct ev_loop *loop, ev_timer *w, int revents)
{
	TimerCallBack::g_event->DeviceTimerHandler(loop, w, revents);
}

#endif
