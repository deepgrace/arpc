//
// Copyright (c) 2023-present DeepGrace (complex dot invoke at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/deepgrace/arpc
//

#define BOOST_ASIO_HAS_IO_URING
#define BOOST_ASIO_DISABLE_EPOLL

#include <thread>
#include <iostream>
#include <arpc.hpp>
#include <ping.pb.h>

namespace net = boost::asio;
namespace gp = google::protobuf;

struct task
{
    arpc::controller controller;

    pb::request request;
    pb::response response;

    arpc::Closure* done;
};

class client
{
public:
    client(net::io_context& ioc, const std::string& host, const std::string& port) : ioc(ioc), work(ioc), host(host), port(port)
    {
        channel = new arpc::channel(ioc);
        service = new pb::service::Stub(channel, pb::service::STUB_OWNS_CHANNEL);
    }

    void ping()
    {
        auto t = std::make_shared<task>();
        auto& controller = t->controller;

        controller.host(host);
        controller.port(port);

        controller.timeout(80);

        t->request.set_command("ping");
        t->done = gp::NewCallback(this, &client::done, t);

        service->execute(&t->controller, &t->request, &t->response, t->done);
    }

    void done(std::shared_ptr<task> t)
    {
        auto& controller = t->controller;

        if (controller.Failed())
            std::cerr << "ErrorCode: " << controller.ErrorCode() << " ErrorText: " << controller.ErrorText() << std::endl;

        std::cout << t->response.DebugString();
    }

    ~client()
    {
        delete service;
    }

private:
    net::io_context& ioc;
    net::io_context::work work;

    arpc::channel* channel;
    pb::service* service;

    std::string host;
    std::string port;
};

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <host> <port>" << std::endl;

        return 1;
    }

    std::string host(argv[1]);
    std::string port(argv[2]);

    net::io_context ioc;
    client c(ioc, host, port);

    std::thread t([&]{ ioc.run(); });

    constexpr size_t size = 100;
    char buff[size];

    while (fgets(buff, size, stdin))
           c.ping();

    t.join();

    return 0;
}
