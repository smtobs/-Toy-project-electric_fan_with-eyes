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
   MqttIface(const std::string& broker_url, const std::string& pub_topic_name,
    			const std::string& cli_id, int qos, int interval, int time_out);
			
    bool ConnectBroker();
    void DisconnectBroker();
    bool IsConnected();
    void Publish(const std::string& topic, const std::string& msg);
    void Subscribe(const std::string& topic);
    std::string GetPubTopicName();
    ~MqttIface();
    
private:
    mqtt::async_client* client;
    callback cb;
    std::string pub_topic_name;
    int qos;
    int time_out;
    int interval;
};

#endif