#ifndef SIGNAL_CALLBACK_H
#define SIGNAL_CALLBACK_H

#include <csignal>
#include <unistd.h>

static void CallBackExit(int signum);

namespace SigCallBack
{
	static EventDriven* g_event;
	void RegisterSignal(EventDriven* event)
	{    
		g_event = event;
		std::signal(SIGINT, CallBackExit);
		std::signal(SIGTERM, CallBackExit);
	}
}

static void CallBackExit(int signum)
{
	if (signum == SIGINT || signum == SIGTERM)
	{
		SigCallBack::g_event->StopEventLoop();
	}
}

#endif
