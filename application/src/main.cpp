#include <iostream>
#include "event_driven.hpp"

int main()
{
    constexpr size_t thread_pool_num = 3;
    EventDriven* ev_driven = new EventDriven(thread_pool_num);
    
    ev_driven->StartEventLoop();
    delete ev_driven;

    return 0;
}
