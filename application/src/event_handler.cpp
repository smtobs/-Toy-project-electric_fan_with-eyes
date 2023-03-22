#include "event_handler.hpp"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

EventHandler::EventHandler()
{
    buzzer         = new Buzzer();
    key_pad        = new KeyPad();
    oled           = new Oled();
    sys_mgr        = new SystemManager();
    mqtt_iface     = new MqttIface();

    this->LockDisplay(0UL);
	
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
            this->InitDisplay();
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
		this->SuccessPwEvent();
            }
            else
            {
		this->FailPwEvent();
            }
            break;

        case CLEAR:
            this->ClearEvent();
            this->InitDisplay();
            buzzer->ButtonPushSound();
            break;

        default: /* Write data to LCD */
            if (this->pw_input_count < 5)
            {
                this->WritePasswordOnDisplay(keypad_data);
                buzzer->ButtonPushSound();
            }
            break;
    }
}

void EventHandler::RenewScreenTimeHandler()
{
    oled->WriteDisplay(sys_mgr->GetLocalTime(), 0, 0);
}

void EventHandler::RetryConBrokerHandler()
{
    if (mqtt_iface->IsConnected() == false)
    {
    	mqtt_iface->DisconnectBroker();
    	mqtt_iface->ConnectBroker();
    	std::cout << "Try Connection Broker" << std::endl;
    }
}

void EventHandler::ClearEvent()
{
    oled->ClearDisplay();
    key_pad->BuffCler();
    this->pw_input_count = 0;
}

void EventHandler::SuccessPwEvent()
{
    this->OpenDoor();
    this->LockDisplay(5000UL);
}

void EventHandler::FailPwEvent()
{
    this->ClearEvent();
    oled->WriteDisplay("Door Open Failed\n", 4, 17);
    this->RenewScreenTimeHandler();
    buzzer->FailSound();
    this->LockDisplay(3000UL);
}

void EventHandler::OpenDoor()
{
    oled->ClearDisplay();                 
    oled->WriteDisplay("Correct Password.\n", 3, 14);
    oled->WriteDisplay("The door opens.\n", 4, 20);
    
    mqtt_iface->Publish("/home/door", this->CreateOpenDoorMsg());
    buzzer->SuccessSound();
}

void EventHandler::InitDisplay()
{
    oled->WriteDisplay("Please enter your\n", 2, 17);
    oled->WriteDisplay("password.\n", 3, 40);
}

void EventHandler::LockDisplay(uint64_t lock_tm)
{
    this->is_init_display = true;
    this->display_lock_tm = lock_tm;
    this->prev_display_lock_tm = sys_mgr->GetTick();
}

void EventHandler::WritePasswordOnDisplay(const char* data)
{ 
    constexpr unsigned char offset = 8;
    if (this->pw_input_count == 0)
    {
        oled->WriteDisplay(data, 5, 45 + (this->pw_input_count * offset));
    }
    else
    {
        oled->WriteDisplay("*", 5, 45 + ((this->pw_input_count - 1) * offset));
        oled->WriteDisplay(data, 5, 45 + (this->pw_input_count * offset));
    }
    this->pw_input_count.fetch_add(1);
}

const char* EventHandler::CreateOpenDoorMsg()
{
	rapidjson::StringBuffer s;
	rapidjson::Writer<rapidjson::StringBuffer> writer(s);

	writer.StartObject();
	writer.Key("order");
	writer.String("open");
	writer.Key("max_open_time");
	writer.Int(5);
	writer.EndObject();
    
	return s.GetString();
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
