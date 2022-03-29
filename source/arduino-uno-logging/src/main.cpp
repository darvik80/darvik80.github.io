#include "Logging.h"

LOG_COMPONENT_SETUP(mqtt)

void setup() {
    Serial.begin(115200);
    logging::CompositeLogger::instance().addLogger(new logging::SerialColorLogger());
    logging::info("setup root");
    mqtt::log::info("setup sub");
}

void loop() {
    logging::debug("debug root: %d", 1);
    delay(1000);
    logging::info("info root: %d", 1);
    delay(1000);
    logging::warning("warning root: %s", "hi");
    delay(1000);
    logging::error("error root: %s", "device overloaded");
    delay(1000);
    logging::critical("critical root: system crached");
    delay(1000);

    mqtt::log::debug("debug: %d", 1);
    delay(1000);
    mqtt::log::info("info: %d", 1);
    delay(1000);
    mqtt::log::warning("warning: %s", "hi");
    delay(1000);
    mqtt::log::error("error: %s", "device overloaded");
    delay(1000);
    mqtt::log::critical("critical: system crached");
    delay(1000);
}