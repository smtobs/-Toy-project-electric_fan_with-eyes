#include "mqtt_callback.hpp"

void callback::connection_lost(const std::string& cause)
{
	std::cout << "Connection lost: " << cause << std::endl;
}

void callback::delivery_complete(mqtt::delivery_token_ptr tok)
{
	std::cout << "Delivery complete for token: "
            << (tok ? tok->get_message_id() : -1) << std::endl;
}

// void callback::message_arrived(const mqtt::const_message_ptr& msg)
// {
// 	std::cout << "Message arrived: " << msg->get_payload_str() << std::endl;
// }
