#include "Logging.h"

LOG_COMPONENT_SETUP(mqtt)

void setup() {
    Serial.begin(115200);
    logging::CompositeLogger::instance().addLogger(new logging::SerialColorLogger());
    logging::info("setup root");
    mqtt::log::info("setup sub");
}

void loop() {
    logging::info("loop root");
    delay(1000);
    mqtt::log::info("loop sub");
    delay(1000);
}