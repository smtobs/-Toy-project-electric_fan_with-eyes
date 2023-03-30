#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "utils.hpp"

using namespace boost;

class ThreadPool : public Utils
{
public:
    ThreadPool(size_t num_threads) : work_(io_service_)
    {
        for (size_t i = 0; i < num_threads; ++i)
        {
            threads_.create_thread(boost::bind(&asio::io_service::run, &io_service_));
            INFO_LOG("{} Thread create..", i);
        }
    }

    template <typename Task>
    void Enqueue(Task task)
    {
        io_service_.post(task);
    }

    ~ThreadPool()
    {
        io_service_.stop();
        threads_.join_all();
        INFO_LOG("Delete..");
    }

private:
    asio::io_service io_service_;
    asio::io_service::work work_;
    boost::thread_group threads_;
};
#endif
