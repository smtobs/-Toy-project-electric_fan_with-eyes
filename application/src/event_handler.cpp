#include "event_handler.hpp"
#include <ctime>


EventHandler::EventHandler()
{
    buzzer         = new Buzzer();
    key_pad        = new KeyPad();
    oled           = new Oled();
    sys_mgr        = new SystemManager();
    mqtt_iface     = new MqttIface();

    this->prev_display_lock_tm = sys_mgr->GetTick();
    this->display_lock_tm = 0UL;
    is_init_display = true;
	
    std::cout << "Create EventHandler" << std::endl;
}

void EventHandler::KeypadEventHandler()
{
    if ((sys_mgr->IsTimeDiff(sys_mgr->GetTick(), this->prev_display_lock_tm, this->display_lock_tm)) == false)
    {
        std::cout << "tiemr...." << std::endl;
        return;
    }
	else
	{
		if (this->is_init_display == true)
		{
            this->ClearEvent();
            this->DisplayInitialScreen();
            this->RenewScreenTimeHandler();
            this->is_init_display = false;
		}
	}

    char keypad_data[2] = {key_pad->Scan(), 0};
    switch (keypad_data[0])
    {
        case NULL_DATA:
            break;

        case CALL:
            this->ClearEvent();
            break;

        case PASSWORD:
            if (sys_mgr->PwCompare(key_pad->BuffCpy()))
            {
                this->OpenDoorEvent();
                buzzer->SuccessSound();
                this->is_init_display = true;
                this->display_lock_tm = 5000UL;
                this->prev_display_lock_tm = sys_mgr->GetTick();
            }
            else
            {
            	std::cout << "Door Open Failed" << std::endl;
            	this->ClearEvent();
            	this->RenewScreenTimeHandler();
            }
            break;

        case CLEAR:
            this->ClearEvent();
            this->DisplayInitialScreen();
            buzzer->ButtonPushSound();
            break;

        default: /* Write data to LCD */
            if (this->pw_input_count < 5)
            {
                this->WritePasswordOnScreen(keypad_data);
                buzzer->ButtonPushSound();
            }
            break;
    }
}

void EventHandler::RenewScreenTimeHandler()
{
    oled->WriteDisplay(sys_mgr->GetLocalTime(), 0, 0);
}

void EventHandler::ClearEvent()
{
    oled->ClearDisplay();
    key_pad->BuffCler();
    this->pw_input_count = 0;
}

void EventHandler::OpenDoorEvent()
{
    oled->ClearDisplay();                 
    oled->WriteDisplay("Correct Password.\n", 3, 14);
    oled->WriteDisplay("The door opens.\n", 4, 20);
}

void EventHandler::DisplayInitialScreen()
{
    oled->WriteDisplay("Please enter your\n", 2, 17);
    oled->WriteDisplay("password.\n", 3, 40);
}

void EventHandler::WritePasswordOnScreen(const char* data)
{ 
    if (this->pw_input_count == 0)
    {
        oled->WriteDisplay(data, 5, 45 + (this->pw_input_count * 8));
    }
    else
    {
        oled->WriteDisplay("*", 5, 45 + ((this->pw_input_count - 1) * 8));
        oled->WriteDisplay(data, 5, 45 + (this->pw_input_count * 8));
    }
    this->pw_input_count.fetch_add(1);
}

EventHandler::~EventHandler()
{
    this->ClearEvent();
    delete buzzer;
    delete key_pad;
    delete oled;
    delete sys_mgr;
    delete mqtt_iface;

    std::cout << "Delete EventHandler" << std::endl;
}
