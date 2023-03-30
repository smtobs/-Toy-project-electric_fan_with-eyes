#ifndef MQTT_IFACE_H
#define MQTT_IFACE_H

#include <MQTTAsync.h>
#include <mqtt/async_client.h>
#include "mqtt_callback.hpp"
#include <string>
#include <iostream>
#include <cstring>

#include "utils.hpp"

class MqttIface : public Utils
{
public:
	callback cb;

    MqttIface(const std::string& broker_url, const std::string& cli_id, int qos, int interval, int time_out);		
    bool ConnectBroker();
    void DisconnectBroker();
    bool IsConnected();
    bool Yield();
    void Publish(const std::string& topic, const std::string& msg);
    void Subscribe(const std::string& topic);
    void Unsubscribe(const std::string& topic);
    ~MqttIface();
    
private:
    mqtt::async_client* client;
    int qos;
    int time_out;
    int interval;
};

#endif