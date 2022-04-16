//
// Created by Ivan Kishchenko on 12.04.2022.
//


#pragma once


#include "module.h"
#include "ir_receiver.h"

#include <MotorDriver.h>

LOG_COMPONENT_SETUP(dc);


class dc_motor : public module, public etl::message_router<dc_motor, ir_receiver_message> {
    MotorDriver _dev;

    int _pos{6};
    int _lastPos{0};
public:
    explicit dc_motor(iregistry &registry) : module(registry),
                                             etl::message_router<dc_motor, ir_receiver_message>(module_dc) {
        registry.message_bus().subscribe(*this);
    }

    void on_receive(const ir_receiver_message &msg) {
        static int SPEED_INDEX[] = {-255, -224, -192, -128, -96, -64, 0, 64, 96, 128, 192, 224, 255};

        switch (msg.btn) {
            case Btn::UP:
                if (++_pos > 12) {
                    _pos = 12;
                }
                break;
            case Btn::DOWN:
                if (--_pos < 0) {
                    _pos = 0;
                }
                break;
            case Btn::ENTER:
                if (_pos < 6) {
                    _pos++;
                } else if (_pos > 6) {
                    _pos--;
                }
                break;
            default:
                return;
        }
        if (_lastPos != _pos) {
            _dev.motor(4, SPEED_INDEX[_pos] > 0 ? FORWARD : BACKWARD, abs(SPEED_INDEX[_pos]));
            _lastPos = _pos;
        }
    }
};


