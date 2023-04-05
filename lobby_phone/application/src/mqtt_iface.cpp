#include "mqtt_iface.hpp"
#include "mqtt/iaction_listener.h"
#include "mqtt/client.h"

MqttIface::MqttIface(const std::string& broker_url, const std::string& cli_id, int qos, int interval, int time_out)
{
    this->client         = new mqtt::async_client(broker_url, cli_id);
    this->qos            = qos;
    this->time_out       = time_out;
    this->interval       = interval;

    this->client->set_callback(this->cb);
}

bool MqttIface::ConnectBroker()
{
    mqtt::connect_options conn_opts;
    conn_opts.set_keep_alive_interval(this->interval);
    conn_opts.set_clean_session(true);
    try
    {   
        mqtt::token_ptr conntok = this->client->connect(conn_opts);
        conntok->wait_for(this->time_out);
    }

    catch (const mqtt::exception& e)
    {
        ERR_LOG("{}", e.what());
        return false;
    }
    return true;
}

void MqttIface::DisconnectBroker()
{
    try
    {
        client->disconnect()->wait();
    }
    catch (const mqtt::exception& e)
    {
        ERR_LOG("{}", e.what());
    }
}

bool MqttIface::IsConnected()
{
	return client->is_connected();
}

bool MqttIface::Yield()
{
    if (this->IsConnected() == false)
    {
        if (this->ConnectBroker() == true)
        {
            INFO_LOG("Connect broker");
        }
        else
        {
            INFO_LOG("Failed connection broker");
        }
    }
    else
    {
        return this->cb.IsMsgArrived();
    }
    return false;
}

void MqttIface::Publish(const std::string& topic, const std::string& msg)
{
    try
    {
        mqtt::message_ptr pubmsg = mqtt::make_message(topic, msg);
        pubmsg->set_qos(this->qos);

        mqtt::delivery_token_ptr pubtok = client->publish(pubmsg);
        pubtok->wait_for(this->time_out);
    }
    catch (const mqtt::exception& e)
    {
        ERR_LOG("{}", e.what());
    }
}

void MqttIface::Subscribe(const std::string& topic)
{
    SubscribeCallback listenr;
    try
    {
        mqtt::token_ptr subtok = client->subscribe(topic, 1, nullptr, listenr);
        this->client->start_consuming();
        subtok->wait();
    }
    catch(const mqtt::exception& e)
    {
        ERR_LOG("{}", e.what());
	    return;
    }
}

void MqttIface::Unsubscribe(const std::string& topic)
{
	mqtt::properties props;
    try
    {
        mqtt::token_ptr unsub_ok = client->unsubscribe(topic, props);
        unsub_ok->wait();
        this->client->stop_consuming();
    }
    catch(const mqtt::exception& e)
    {
        ERR_LOG("{}", e.what());
	    return;
    }
}

MqttIface::~MqttIface()
{
    this->DisconnectBroker();
    delete this->client;
}
