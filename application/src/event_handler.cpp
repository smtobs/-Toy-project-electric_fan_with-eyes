#include "event_handler.hpp"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

EventHandler::EventHandler(ConfigManager& config_)
{
    config = &config_;

    int int_val         = config->int_val;
    std::string str_val = config->str_val;

    /* Buzzer Driver Open */
    std::string ret_config = config->GetConfigVal("DRIVERS", "BUZZER_PATH", str_val);
    buzzer         = new Buzzer(ret_config.c_str());
      
    /* Keypad Driver Open */
    ret_config = config->GetConfigVal("DRIVERS", "KEYPAD_PATH", str_val);
    key_pad        = new KeyPad(ret_config.c_str());
    
    /* Oled Driver Open */
    ret_config     = config->GetConfigVal("DRIVERS", "OLED_PATH", str_val);
    oled           = new Oled(ret_config.c_str());
    
    /* Create System Manager */
    sys_mgr        = new SystemManager(config_);

    /* Create Mqtt Interface */
    mqtt_iface     = new MqttIface(config->GetConfigVal("MQTT", "BROKER_URL", str_val),
                                    config->GetConfigVal("MQTT", "CLI_ID", str_val),
                                    config->GetConfigVal("MQTT", "QOS", int_val), 
                                    config->GetConfigVal("MQTT", "INTERVAL", int_val),
                                    config->GetConfigVal("MQTT", "TIME_OUT", int_val));

    /* Create image_sender */
    image_sender   = new ImageSender(config->GetConfigVal("VIDEO", "READ_PATH", str_val),
                                config->GetConfigVal("VIDEO", "DEST_IP", str_val),
                                config->GetConfigVal("VIDEO", "DEST_PORT", int_val),
                                config->GetConfigVal("VIDEO", "IMG_WIDTH", int_val),
                                config->GetConfigVal("VIDEO", "IMG_HEIGHT", int_val));

    this->LockDisplay(0UL);

    INFO_LOG("Create eventHandler");
}

void EventHandler::KeypadEventHandler()
{
    /* Check Keypad Lock*/
    if ((sys_mgr->IsTimeDiff(sys_mgr->GetTick(), this->prev_display_lock_tm, this->display_lock_tm)) == false)
    {
        DEBUG_LOG("keypad lock..");
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

    /* Check Keypad Input */
    char keypad_data[2] = {key_pad->Scan(), 0};
    switch (static_cast<KeyPad::Data>(keypad_data[0]))
    {
        case KeyPad::Data::INVALID:
            break;

        case KeyPad::Data::CALL:
            this->IntercomEvent();
            break;

        case KeyPad::Data::PASSWORD:
            if (sys_mgr->PwCompare(key_pad->BuffCpy()))
            {
                this->SuccessPwEvent();
            }
            else
            {
                this->FailPwEvent();
            }
            break;

        case KeyPad::Data::CLEAR:
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
            else
            {
                buzzer->FailSound();
            }
            break;
    }
}

void EventHandler::IntercomEvent()
{
    this->LockDisplay(10000UL);
    oled->ClearDisplay();                 
    oled->WriteDisplay("Requesting permission to enter..\n", 3, 1);
    
    /* Send Image */
    std::string dest_ip = config->GetConfigVal("VIDEO", "DEST_IP", config->str_val);
    image_sender->SendImage(dest_ip.c_str(),
                            config->GetConfigVal("VIDEO", "DEST_PORT", config->int_val),
                            config->GetConfigVal("VIDEO", "IMG_WIDTH", config->int_val),
                            config->GetConfigVal("VIDEO", "IMG_HEIGHT", config->int_val));

    /* Topic subscribe */
    mqtt_iface->Subscribe(config->GetConfigVal("MQTT", "SUB_TOPIC_NAME", config->str_val));
}

void EventHandler::RenewScreenTimeHandler()
{
    oled->WriteDisplay(sys_mgr->GetLocalTime(), 0, 0);
}

void EventHandler::MqttYieldEventHandler()
{
    bool is_msg = mqtt_iface->Yield();
    if (is_msg)
    {
        std::string topic_msg = mqtt_iface->cb.GetSubTopicMsg();
        mqtt_iface->cb.ClearSubTopicMsg();
        
        /* Open Door */
        if (this->IsEqual(topic_msg, static_cast<std::string>("allow")))
        {
            this->AllowAccessEvent();
            mqtt_iface->Unsubscribe(config->GetConfigVal("MQTT", "SUB_TOPIC_NAME", config->str_val));
        }
        else if (this->IsEqual(topic_msg, static_cast<std::string>("deny")))
        {
            this->DenyAccessEvent();
            mqtt_iface->Unsubscribe(config->GetConfigVal("MQTT", "SUB_TOPIC_NAME", config->str_val));
        }
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
    this->LockDisplay(5000UL);
    oled->ClearDisplay();                 
    oled->WriteDisplay("Correct Password.\n", 3, 14);
    this->RenewScreenTimeHandler();
    this->OpenDoor();
}

void EventHandler::FailPwEvent()
{
    this->LockDisplay(3000UL);
    this->ClearEvent();
    oled->WriteDisplay("Door Open Failed\n", 4, 17);
    this->RenewScreenTimeHandler();
    buzzer->FailSound();
}

void EventHandler::AllowAccessEvent()
{
    this->LockDisplay(5000UL);
    this->ClearEvent();
    this->OpenDoor();
}

void EventHandler::DenyAccessEvent()
{
    this->LockDisplay(3000UL);
    this->ClearEvent();
    oled->WriteDisplay("Access Denied\n", 4, 17);
    this->RenewScreenTimeHandler();	
}

void EventHandler::OpenDoor()
{
    oled->WriteDisplay("The door opens.\n", 4, 20);
    mqtt_iface->Publish(config->GetConfigVal("MQTT", "PUB_TOPIC_NAME", config->str_val), this->CreateOpenDoorMsg());
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
	static rapidjson::StringBuffer s;
    memset(&s, 0x0, sizeof(s));

	rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    try
    {
        writer.StartObject();
        writer.Key("order");
        writer.String("open");
        writer.Key("max_open_time");
        writer.Int(5);
        writer.EndObject();
    } 
    catch (const std::exception& e)
    {
        ERR_LOG("{}", e.what());
        return nullptr;
    }    
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
    delete image_sender;

    INFO_LOG("Delete eventHandler..");
}
