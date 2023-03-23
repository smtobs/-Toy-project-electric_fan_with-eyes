#include "mqtt_iface.hpp"

MqttIface::MqttIface(const std::string& broker_url, const std::string& pub_topic_name_, const std::string& cli_id,
			int qos, int interval, int time_out)
{
    this->client = new mqtt::async_client(broker_url, cli_id);
    this->client->set_callback(this->cb);
    
    this->qos            = qos;
    this->time_out       = time_out;
    this->interval       = interval;
    this->pub_topic_name = pub_topic_name_;
}

bool MqttIface::ConnectBroker()
{
    try
    {   
        //mqtt::async_client client = this->CreateAsynClient();
        mqtt::connect_options conn_opts;
        conn_opts.set_keep_alive_interval(this->interval);
        conn_opts.set_clean_session(true);

        mqtt::token_ptr conntok = this->client->connect(conn_opts);
        conntok->wait_for(this->time_out);
    }
    catch (const mqtt::exception& e)
    {
        std::cerr << e.what() << std::endl;
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
        std::cerr << e.what() << std::endl;
    }
}

bool MqttIface::IsConnected()
{
	return client->is_connected();
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
        std::cerr << e.what() << std::endl;
    }
}

std::string MqttIface::GetPubTopicName()
{
	return this->pub_topic_name;
}

void MqttIface::Subscribe(const std::string& topic)
{
    // try
    // {
    //     mqtt::subscribe_options subOpts;
    //     subOpts.set_qos(this->QOS);

    //     mqtt::token_ptr subtok = client.subscribe(topic, subOpts);
    //     subtok->wait_for_completion();
    // }
    // catch(const mqtt::exception& e)
    // {
    //     std::cerr << e.what() << std::endl;
    // }
}

MqttIface::~MqttIface()
{
    this->DisconnectBroker();
    delete this->client;
}
