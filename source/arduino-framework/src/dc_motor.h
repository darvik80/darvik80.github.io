//
// Created by Ivan Kishchenko on 12.04.2022.
//


#pragma once


#include "module.h"
#include "ir_receiver.h"

LOG_COMPONENT_SETUP(dc);

class dc_motor : public module, public etl::message_router<dc_motor, ir_receiver_message> {
public:
    explicit dc_motor(iregistry &registry) : module(registry),
                                             etl::message_router<dc_motor, ir_receiver_message>(module_dc) {
        registry.message_bus().subscribe(*this);
    }

    void on_receive(const ir_receiver_message& msg) {
        dc::log::info("ir: 0x%08lx", msg.data);
    }
};


