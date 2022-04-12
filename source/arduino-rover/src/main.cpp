#include <module.h>

#include "ir_receiver.h"
#include "dc_motor.h"

int RECV_PIN = 11; // define input pin on Arduino

class main_app : public application<2> {
public:
    main_app() = default;

    void setup() override {
        Serial.begin(115200);
        logging::add_logger(new logging::serial_logger());

        registry::add(new ir_receiver(*this, RECV_PIN));
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