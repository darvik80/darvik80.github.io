#include <module.h>

#include "ir_receiver.h"
#include "dc_motor.h"
#include "servo_motor.h"

class main_app : public application<3> {
public:
    main_app() = default;

    void setup() override {
        Serial.begin(115200);
        logging::add_logger(new logging::serial_logger());

        registry::add(new ir_receiver(*this, 11));
        registry::add(new servo_motor(*this, 10));
        registry::add(new dc_motor(*this));

        application::setup();
    }
};

main_app app;

void setup() {
    app.setup();
}

void loop() {
    app.loop();
    delay(100);
}