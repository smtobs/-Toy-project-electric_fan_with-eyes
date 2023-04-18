#include <csignal>
#include <unistd.h>
#include <cstdlib>

#include "event_callback.hpp"

namespace EventCallback
{
	EventDriven* g_event;

	uv_signal_t sigint_handle;
	uv_signal_t sigterm_handle;

	uv_timer_t keypad_timer;
	uv_timer_t oled_timer;
	uv_timer_t mqtt_timer;

	static constexpr uint16_t KEYPAD_EVENT = 0u;
	static constexpr uint16_t TIME_UPDATE_EVENT = 1u;
	static constexpr uint16_t MQTT_YIELD_EVENT = 2u;

static void SigCallback(uv_signal_t* handle, int signum)
{
	g_event->StopEventLoop();
}

static void EventListener(uv_work_t* req)
{
	uint16_t event = *(uint16_t *) req->data;

	if (!req || !g_event || !g_event->handle)
	{
		
		goto err;
	}

	switch (event)
	{
		case KEYPAD_EVENT:
			g_event->handle->KeypadEventHandler();
			break;

		case TIME_UPDATE_EVENT:
			g_event->handle->RenewScreenTimeHandler();
			break;

		case MQTT_YIELD_EVENT:
			g_event->handle->MqttYieldEventHandler();
			break;

		default:
			break;
	}

	err:
		delete req;
}

static void RespEventListener(uv_work_t* req, int status)
{
	/* Todo */
}

static void KeypadTimerCallback(uv_timer_t* w)
{
	EventQueue(KEYPAD_EVENT);
}

static void RenewOledTimeCallback(uv_timer_t* w)
{
	EventQueue(TIME_UPDATE_EVENT);
}

static void MqttYieldCallback(uv_timer_t* w)
{
	EventQueue(MQTT_YIELD_EVENT);
}

void EventQueue(uint16_t event_flag)
{
	uv_work_t* req = new uv_work_t();
	req->data = (void *)&event_flag;
	uv_queue_work(g_event->event_loop, req, EventListener, RespEventListener);
}

void UnregisterEventCallback()
{
	g_event = NULL;

	uv_timer_stop(&keypad_timer);
	uv_close((uv_handle_t* )&keypad_timer, NULL);

	uv_timer_stop(&oled_timer);
	uv_close((uv_handle_t* )&oled_timer, NULL);

	uv_timer_stop(&mqtt_timer);
	uv_close((uv_handle_t* )&mqtt_timer, NULL);
}

void RegisterEventCallback(EventDriven* event)
{
	g_event = event;

	uv_signal_init(g_event->event_loop, &sigint_handle);
	uv_signal_init(g_event->event_loop, &sigterm_handle);

	uv_signal_start(&sigint_handle, SigCallback, SIGINT);
	uv_signal_start(&sigterm_handle, SigCallback, SIGTERM);

	/* Register Check Keypad Input Timer */
	uv_timer_init(g_event->event_loop, &keypad_timer);
	uv_timer_start(&keypad_timer, KeypadTimerCallback, 1000, 100);

	/* Register Oled Time Update Timer */
	uv_timer_init(g_event->event_loop, &oled_timer);
	uv_timer_start(&oled_timer, RenewOledTimeCallback, 100, 1000);

	/* Register Mqtt yileld Timer */
	uv_timer_init(g_event->event_loop, &mqtt_timer);
	uv_timer_start(&mqtt_timer, MqttYieldCallback, 200, 1500);
}
};

