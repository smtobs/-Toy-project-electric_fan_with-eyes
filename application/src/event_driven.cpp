#include "event_driven.hpp"
#include "timer_callback.hpp"
#include "signal_callback.hpp"

EventDriven::EventDriven(const YAML::Node& config)
{
    this->pool    = new ThreadPool(3);
    this->handle  = new EventHandler(config);
    
    this->event_loop = ev_default_loop(0);
    TimerCallBack::RegisterTimer(this);
    SigCallBack::RegisterSignal(this);
}

void EventDriven::StartEventLoop()
{
    if (this->is_event_loop_running == false)
    {
        ev_timer_start(this->event_loop, &this->device_timer);
        ev_timer_start(this->event_loop, &this->network_timer);
        ev_loop(this->event_loop, 0);
    }
    else
    {
        std::cout << "An event loop is already running." << std::endl;
    }
}

void EventDriven::StopEventLoop()
{
    ev_timer_stop(this->event_loop, &this->device_timer);
    ev_timer_stop(this->event_loop, &this->network_timer);
    this->is_event_loop_running = false;    
    ev_break(this->event_loop, EVBREAK_ALL);
}

void EventDriven::DeviceEventListener(uint16_t event_flag)
{	
    if (event_flag & (1u << KEYPAD_EVENT))
    {
    	handle->KeypadEventHandler();
    }

    if (event_flag & (1u << TIME_UPDATE_EVENT))
    {
        handle->RenewScreenTimeHandler();
    }
}

void EventDriven::DeviceTimerHandler(struct ev_loop *loop, ev_timer* w, int revents)
{
    static uint16_t cnt[MAX_EVENT] = {0u,};
    uint16_t event_flag = 0x0; 

    if ((++cnt[KEYPAD_EVENT]) >= KEYPAD_EVENT_PERIOD)
    {
        event_flag |= 1u << KEYPAD_EVENT;
        cnt[KEYPAD_EVENT] = 0u;
    }

    if ((++cnt[TIME_UPDATE_EVENT]) >= TIME_UPDATE_EVENT_PERIOD)
    {
       event_flag |= 1u << TIME_UPDATE_EVENT;
       cnt[TIME_UPDATE_EVENT] = 0u;
    }
    pool->Enqueue(boost::bind(&EventDriven::DeviceEventListener, this, event_flag));
}

void EventDriven::NetworkTimerHandler(struct ev_loop *loop, ev_timer *w, int revents)
{
    static uint8_t cnt[MAX_NETWORK_EVENT] = {0u,};
    uint16_t event_flag = 0x0;

    if ((++cnt[BROKER_RETRY_CON_EVENT]) >= BROKER_RETRY_CON_EVENT_PERIOD)
    {
       event_flag |= 1u << BROKER_RETRY_CON_EVENT;
       cnt[BROKER_RETRY_CON_EVENT] = 0u;
    }

  //  if ((++cnt[GET_WIFI_SIG_EVENT]) >= GET_WIFI_SIG_EVENT_PERIOD)
    //{
      // event_flag |= 1u << GET_WIFI_SIG_EVENT;
   // }
    pool->Enqueue(boost::bind(&EventDriven::NetworkEventListener, this, event_flag));
}

void EventDriven::NetworkEventListener(uint16_t event_flag)
{
    if (event_flag & (1u << BROKER_RETRY_CON_EVENT))
    {
        handle->RetryConBrokerHandler();
    }

    //if (event_flag & (1u << GET_WIFI_SIG_EVENT))
    //{
        //this->RenewScreenTimeHandler();
    //}
}

void EventDriven::StartTimer()
{

}
void EventDriven::StopTimer()
{

}

EventDriven::~EventDriven()
{
    delete this->pool;
    delete this->handle;
}
