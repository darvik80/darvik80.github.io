#include "Application.h"
#include "esp_log.h"

Application::Application()
{
    ESP_LOGI("app", "Constructor");
}

Application::~Application()
{
    ESP_LOGI("app", "Destructor");
}