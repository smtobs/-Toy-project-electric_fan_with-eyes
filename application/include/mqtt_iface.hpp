#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <MQTTAsync.h>
#include "mqtt/async_client.h"
#include "mqtt_callback.hpp"
#include <string>
#include <iostream>
#include <cstring>

class MqttIface : public virtual mqtt::callback
{
public:
    MqttIface();
    bool ConnectBroker();
    void DisconnectBroker();
    void Publish(const std::string& topic, const std::string& msg);
    void Subscribe(const std::string& topic);
    ~MqttIface();
    
private:
    mqtt::async_client* client;
    callback cb;
    const std::string BROKER_ADDRESS = "tcp://192.168.219.103:1883";
    const std::string CLIENT_ID      = "access_control_system";
    const int QOS                    = 1;
    const int TIMEOUT                = 1000;
};

#endif