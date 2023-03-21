#include <iostream>
#include "event_driven.hpp"

int main()
{
    EventDriven* ev_driven = new EventDriven(4);
    ev_driven->StartEventLoop();
    delete ev_driven;

    return 0;
}
