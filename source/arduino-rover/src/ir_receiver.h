//
// Created by Ivan Kishchenko on 12.04.2022.
//


#pragma once


#include "module.h"
#include "messaging.h"

#define RAW_BUFFER_LENGTH 100

#include <IRremote.hpp>

LOG_COMPONENT_SETUP(ir);

enum class Btn {
    UNK,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER
};

struct ir_receiver_message : public etl::message<msg_ir> {
    explicit ir_receiver_message(Btn btn) : btn(btn) {}

    Btn btn;
};

class ir_receiver : public module, public etl::message_producer {
    IRrecv _ir;
    uint32_t _lastData{};
private:
    static Btn code2Btn(uint32_t code) {
        switch (code) {
            case 0xb946ff00:
                return Btn::UP;
            case 0xea15ff00:
                return Btn::DOWN;
            case 0xbc43ff00:
                return Btn::LEFT;
            case 0xbb44ff00:
                return Btn::RIGHT;
            case 0xbf40ff00:
                return Btn::ENTER;
            default:
                break;
        }

        return Btn::UNK;
    }

    static const char* code2Str(uint32_t code) {
        switch (code) {
            case 0xb946ff00:
                return "UP";
            case 0xea15ff00:
                return "DOWN";
            case 0xbc43ff00:
                return "LEFT";
            case 0xbb44ff00:
                return "RIGHT";
            case 0xbf40ff00:
                return "OK";
            default:
                break;
        }

        return "IGN";
    }
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
                ir::log::info("ir: %s", code2Str(_lastData));

                etl::send_message(
                        registry.message_bus(),
                        ir_receiver_message{code2Btn(_lastData)}
                );

                _ir.resume();
            }
        }
    }
};


