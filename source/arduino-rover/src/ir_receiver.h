//
// Created by Ivan Kishchenko on 12.04.2022.
//


#pragma once


#include "module.h"
#include "messaging.h"

#define RAW_BUFFER_LENGTH 100

#include <IRremote.hpp>

LOG_COMPONENT_SETUP(ir);

struct ir_receiver_message : public etl::message<msg_ir> {
    ir_receiver_message(decode_type_t type, uint32_t data) : type(type), data(data) {}

    decode_type_t type;
    uint32_t data{};
};

class ir_receiver : public module, public etl::message_producer {
    IRrecv _ir;
    uint32_t _lastData{};
public:
    explicit ir_receiver(iregistry &registry, int pin) : module(registry),
                                                         message_producer(module_ir),
                                                         _ir(pin) {
    }

    void setup(iregistry &registry) override {
        _ir.enableIRIn();
        registry.message_bus().subscribe(*this);
    }

    void loop(iregistry &registry) override {
        if (_ir.available()) {
            auto data = _ir.read();
            if (data) {
                if (data->decodedRawData) {
                    _lastData = data->decodedRawData;
                }
                etl::send_message(
                        registry.message_bus(),
                        ir_receiver_message{data->protocol, _lastData}
                );

                _ir.resume();
            }
        }
    }
};


