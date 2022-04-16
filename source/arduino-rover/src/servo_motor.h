//
// Created by Ivan Kishchenko on 12.04.2022.
//


#pragma once


#include "module.h"
#include "ir_receiver.h"
#include <Servo.h>

LOG_COMPONENT_SETUP(servo);

#define SERVO_STEP 10

class servo_motor : public module, public etl::message_router<servo_motor, ir_receiver_message> {
    Servo _dev;

    int _angle = 90;
    int _lastAngle = 0;
public:
    explicit servo_motor(iregistry &registry, int pin) : module(registry),
                                             etl::message_router<servo_motor, ir_receiver_message>(module_dc) {
        registry.message_bus().subscribe(*this);
        _dev.attach(pin);
    }

    void on_receive(const ir_receiver_message& msg) {
        switch (msg.btn) {
            case Btn::LEFT:
                _angle -= SERVO_STEP;
                break;
            case Btn::RIGHT:
                _angle += SERVO_STEP;
                break;
            case Btn::ENTER:
                _angle = 90;
                break;
            default:
                return;
        }

        if (_angle > 150) {
            _angle = 150;
        } else if (_angle < 30) {
            _angle = 30;
        }

        if (_lastAngle != _angle) {
            _dev.write(_angle);
            _lastAngle = _angle;
        }
    }
};


