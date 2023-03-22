#include "mqtt_iface.hpp"

MqttIface::MqttIface()
{
    this->client = new mqtt::async_client(this->BROKER_ADDRESS, this->CLIENT_ID);
    this->client->set_callback(this->cb);
}

bool MqttIface::ConnectBroker()
{
    try
    {   
        //mqtt::async_client client = this->CreateAsynClient();
        mqtt::connect_options conn_opts;
        conn_opts.set_keep_alive_interval(20);
        conn_opts.set_clean_session(true);

        mqtt::token_ptr conntok = this->client->connect(conn_opts);
        conntok->wait_for(this->TIMEOUT);
    }
    catch(const mqtt::exception& e)
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
    catch(const mqtt::exception& e)
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
        pubmsg->set_qos(this->QOS);

        mqtt::delivery_token_ptr pubtok = client->publish(pubmsg);
        pubtok->wait_for(this->TIMEOUT);
    }
    catch(const mqtt::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
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
