//
// Created by Ivan Kishchenko on 16.09.2021.
//

#pragma once

#include <Arduino.h>

#define DECLARE_LEVEL(LEVEL)                                       \
    static void LEVEL(const char *fmt, ...) {                      \
        char buf[64];                                              \
        va_list args;                                              \
        va_start(args, fmt);                                       \
        vsprintf(buf, fmt, args);                                  \
        va_end(args);                                              \
        composite_logger::instance().l(level::LEVEL, nullptr, buf); \
    }

namespace logging {
enum class level { debug,
                   info,
                   warning,
                   error,
                   critical };

class logger {
  public:
    logger *_next = nullptr;

  public:
    void log(level lvl, const char *fmt, ...) {
        char buf[64];
        va_list args;
        va_start(args, fmt);
        vsprintf(buf, fmt, args);
        va_end(args);
        l(lvl, nullptr, (const char *)buf);
    }

    virtual void l(level lvl, const char *module, const char *message) = 0;

    virtual ~logger() = default;
};

class serial_logger : public logger {
  public:
    void l(level lvl, const char *module, const char *message) override {
        Serial.print(millis());
        switch (lvl) {
        case level::debug:
            Serial.print(" [debug]");
            break;
        case level::info:
            Serial.print(" [ info]");
            break;
        case level::warning:
            Serial.print(" [ warn]");
            break;
        case level::error:
            Serial.print(" [error]");
            break;
        case level::critical:
            Serial.print(" [ crit]");
            break;
        default:
            Serial.print(" [ unk]");
            break;
        }

        Serial.print(" [");
        if (module) {
            Serial.print(module);
        } else {
            Serial.print("app");
        }
        Serial.print("]: ");

        Serial.println(message);
    }
};

class serial_color_logger : public serial_logger {
  public:
    void l(level lvl, const char *module, const char *message) override {
        Serial.print("\033[38;5;15m");
        Serial.print(millis());
        switch (lvl) {
        case level::debug:
            Serial.print(" [\033[1;37mdebug");
            break;
        case level::info:
            Serial.print(" [\033[1;32m info");
            break;
        case level::warning:
            Serial.print(" [\033[1;33m warn");
            break;
        case level::error:
            Serial.print(" [\033[1;31merror");
            break;
        case level::critical:
            Serial.print(" [\033[1;31m crit");
            break;
        default:
            Serial.print(" [\033[1;31m  unk");
            break;
        }
        Serial.print("\033[38;5;15m]");

        Serial.print(" [\033[34m");
        if (module) {
            Serial.print(module);
        } else {
            Serial.print("app");
        }
        Serial.print("\033[38;5;15m]: ");

        Serial.println(message);
    }
};

class composite_logger : public logger {
    level _level{level::info};
    logger *_root{nullptr};

  private:
    composite_logger() = default;

    composite_logger(composite_logger &dup) = default;

  public:
    static composite_logger &instance() {
        static composite_logger inst;
        return inst;
    }

    void l(level lvl, const char *module, const char *message) override {
        if (lvl >= _level) {
            logger *logger = _root;
            while (logger) {
                logger->l(lvl, module, message);
                logger = logger->_next;
            }
        }
    }

    void set_level(level lvl) { _level = lvl; }

    void add_logger(logger *log) {
        if (!_root) {
            _root = log;
        } else {
            logger *logger = _root;
            while (logger && logger->_next) {
                logger = logger->_next;
            }

            logger->_next = log;
        }
    }
};

void inline add_logger(logger *log) {
    composite_logger::instance().add_logger(log);
}

void inline set_log_level(level lvl) {
    composite_logger::instance().set_level(lvl);
}

DECLARE_LEVEL(debug);

DECLARE_LEVEL(info);

DECLARE_LEVEL(warning);

DECLARE_LEVEL(error);

DECLARE_LEVEL(critical);
} // namespace logging

#define DECLARE_COMPONENT_LEVEL(COMPONENT, LEVEL)                                       \
    static void LEVEL(const char *fmt, ...) {                                           \
        char buf[64];                                                                   \
        va_list args;                                                                   \
        va_start(args, fmt);                                                            \
        vsprintf(buf, fmt, args);                                                       \
        va_end(args);                                                                   \
        logging::composite_logger::instance().l(logging::level::LEVEL, #COMPONENT, buf); \
    }

#define LOG_COMPONENT_SETUP(COMPONENT)           \
    namespace COMPONENT {                        \
    namespace log {                              \
                                                 \
    DECLARE_COMPONENT_LEVEL(COMPONENT, debug)    \
                                                 \
    DECLARE_COMPONENT_LEVEL(COMPONENT, info)     \
                                                 \
    DECLARE_COMPONENT_LEVEL(COMPONENT, warning)  \
                                                 \
    DECLARE_COMPONENT_LEVEL(COMPONENT, error)    \
                                                 \
    DECLARE_COMPONENT_LEVEL(COMPONENT, critical) \
    }                                            \
    }
