// -*- mode:c++; indent-tabs-mode:t; tab-width:4; c-basic-offset:2; -*-
// vi: noet ts=4 sw=2

// Copyright (C) 2016 SBI BITS Japan 

#include <pthread.h>
#include <unistd.h>
#include <cassert>
#include <string>
#include <iostream>
#include <zmq.hpp>

void *workerFn(void *arg) {
    zmq::context_t *context = (zmq::context_t *) arg;

    zmq::socket_t socket(*context, ZMQ_REP);
    socket.connect("inproc://workers");

    while (true) {
        //  Wait for next request from client
        zmq::message_t req;
        socket.recv(&req);
        std::cout << "received request: [" << (char*) req.data() << "]" << std::endl;

        zmq::message_t rep(256);
        sprintf(static_cast<char*>(rep.data()), "tid(%08lx) ack {%.*s}",
				pthread_self(), static_cast<int>(req.size()),
				static_cast<const char*>(req.data()));
        socket.send(rep);

        //  Do some 'work'
        sleep (1);
    }
    return (NULL);
}

int main (int, const char *[]) {
    //  Prepare our context and sockets
    zmq::context_t context(1);
    zmq::socket_t clients(context, ZMQ_ROUTER);
    clients.bind("tcp://*:5432");
    zmq::socket_t workers(context, ZMQ_DEALER);
    workers.bind("inproc://workers");

    //  Launch pool of worker threads
    for (int threads = 0; threads < 5; threads++) {
        pthread_t w;
        pthread_create(&w, NULL, workerFn, (void *) &context);
    }

    //  Connect work threads to client threads via a queue
    zmq::proxy(static_cast<void*>(clients), static_cast<void*>(workers), NULL);
    return 0;
}
