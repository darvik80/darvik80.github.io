//
// Created by Ivan Kishchenko on 16.09.2021.
//

#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>

#define DECLARE_LEVEL(LEVEL)                                                        \
    static void LEVEL(const char *fmt, ...) {                                       \
        char buf[64];                                                               \
        va_list args;                                                               \
        va_start(args, fmt);                                                        \
        vsprintf(buf, fmt, args);                                                   \
        va_end(args);                                                               \
        CompositeLogger::instance().l(level::LEVEL, nullptr, buf);                  \
    }

namespace logging {
    enum class level {
        debug, info, warning, error, critical
    };

    class Logger {
    public:
        Logger *_next = nullptr;

    public:
        void log(level lvl, const char *fmt, ...) {
            char buf[64];
            va_list args;
            va_start(args, fmt);
            vsprintf(buf, fmt, args);
            va_end(args);
            l(lvl, nullptr, (const char *) buf);
        }

        virtual void l(level lvl, const char *module, const char *message) = 0;

        virtual ~Logger() = default;
    };

    class SerialLogger : public Logger {
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

    class SerialColorLogger : public SerialLogger {
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

    class CompositeLogger : public Logger {
        level _level{level::info};
        Logger *_root{nullptr};

    private:
        CompositeLogger() = default;

        CompositeLogger(CompositeLogger &dup) = default;

    public:
        static CompositeLogger &instance() {
            static CompositeLogger inst;
            return inst;
        }

        void l(level lvl, const char *module, const char *message) override {
            if (lvl >= _level) {
                Logger *logger = _root;
                while (logger) {
                    logger->l(lvl, module, message);
                    logger = logger->_next;
                }
            }
        }

        void setLevel(level lvl) { _level = lvl; }

        void addLogger(Logger *log) {
            if (!_root) {
                _root = log;
            } else {
                Logger *logger = _root;
                while (logger && logger->_next) {
                    logger = logger->_next;
                }

                logger->_next = log;
            }
        }
    };

    void inline addLogger(Logger *log) {
        CompositeLogger::instance().addLogger(log);
    }

    void inline setLogLevel(level lvl) {
        CompositeLogger::instance().setLevel(lvl);
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
        logging::CompositeLogger::instance().l(logging::level::LEVEL, #COMPONENT, buf); \
    }

#define LOG_COMPONENT_SETUP(COMPONENT)                                         \
  namespace COMPONENT {                                                        \
  namespace log {                                                              \
                                                                               \
  DECLARE_COMPONENT_LEVEL(COMPONENT, debug)                                    \
                                                                               \
  DECLARE_COMPONENT_LEVEL(COMPONENT, info)                                     \
                                                                               \
  DECLARE_COMPONENT_LEVEL(COMPONENT, warning)                                  \
                                                                               \
  DECLARE_COMPONENT_LEVEL(COMPONENT, error)                                    \
                                                                               \
  DECLARE_COMPONENT_LEVEL(COMPONENT, critical)                                 \
  }                                                                            \
  }
