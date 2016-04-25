// -*- mode:c++; indent-tabs-mode:t; tab-width:4; c-basic-offset:2; -*-
// vi: noet ts=4 sw=2

// Copyright (C) 2016 SBI BITS Japan 

#include <pthread.h>
#include <unistd.h>
#include <cassert>
#include <string>
#include <iostream>
#include <zmq.hpp>

int main (int argc, const char *argv[]) {
    //  Prepare our context and sockets
    zmq::context_t context(1);
    zmq::socket_t client(context, ZMQ_REQ);
    client.connect("tcp://127.0.0.1:5432");

    for (int i = 1; i < argc; ++i) {
        std::cout << "send: " << argv[i] << std::endl;
        client.send(argv[i], strlen(argv[i]));
        zmq::message_t r;
        client.recv(&r);
        std::cout << "recv: " << std::string(static_cast<char*>(r.data()), r.size()) << std::endl;
    }

    return 0;
}
