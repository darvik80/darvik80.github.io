//
// Created by Ivan Kishchenko on 12.04.2022.
//


#pragma once

#define ETL_NO_STL

#include <etl/profiles/arduino_arm.h>
#include <Arduino.h>
#include <string.h>

#include "etl/message_bus.h"
#include "etl/message_router_registry.h"
#include "messaging.h"
#include "logging.h"

class iregistry {
public:
    template<typename C>
    C *module(uint8_t id) {
        auto router = find(id);
        return router ? (C *) router : nullptr;
    }

    virtual etl::imessage_router *find(uint8_t id) = 0;

    virtual etl::imessage_bus &message_bus() = 0;

    virtual etl::imessage_bus &sys_bus() = 0;
};

class imodule {
public:
    virtual void setup(class iregistry &registry) = 0;

    virtual void loop(class iregistry &registry) = 0;
};

template<size_t MaxRouters>
class registry : public iregistry, public etl::message_router_registry<MaxRouters> {
    etl::message_bus<MaxRouters> _bus;
    etl::message_bus<MaxRouters> _sys;
public:
    etl::imessage_router *find(uint8_t id) override {
        return etl::message_router_registry<MaxRouters>::find(id);
    }

    etl::imessage_bus &message_bus() override {
        return _bus;
    }

    etl::imessage_bus &sys_bus() override {
        return _sys;
    }
};

class system_consumer : public etl::message_router<system_consumer, message_setup, message_loop> {
    imodule& _module;
public:
    explicit system_consumer(imodule &module) : message_router(ALL_MESSAGE_ROUTERS), _module(module) {
    }

    // etl::message_router stub
    void on_receive_unknown(const etl::imessage &msg) {}

    void on_receive(const message_setup &msg) {
        _module.setup(*msg.registry);
    }

    void on_receive(const message_loop &msg) {
        _module.loop(*msg.registry);
    }
};

class module : public imodule {
    system_consumer _consumer;
public:
    explicit module(iregistry &registry) : _consumer(*this) {
        registry.sys_bus().subscribe(_consumer);
    }

    void setup(iregistry &registry) override {
    }

    void loop(iregistry &registry) override {

    }

    // etl::message_router stub
    void on_receive_unknown(const etl::imessage &msg) {}
};

LOG_COMPONENT_SETUP(root);

template<size_t MaxRouters>
class application : public registry<MaxRouters> {
public:
    virtual void setup() {
        etl::send_message(registry<MaxRouters>::sys_bus(), message_setup{this});
    }

    virtual void loop() {
        etl::send_message(registry<MaxRouters>::sys_bus(), message_loop{this});
        delay(100);
    }

    void on_receive_unknown(const etl::imessage &msg) {}
};
