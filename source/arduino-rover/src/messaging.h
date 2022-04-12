//
// Created by Ivan Kishchenko on 12.04.2022.
//
#pragma once

enum message_id {
    msg_setup,
    msg_loop,
    msg_ir,
};

enum message_module {
    module_ir,
    module_dc,
};

struct message_setup : public etl::message<msg_setup> {
    explicit message_setup(class iregistry *registry) : registry(registry) {}

    class iregistry* registry{};
};

struct message_loop : public etl::message<msg_loop> {
    explicit message_loop(class iregistry *registry) : registry(registry) {}

    class iregistry* registry{};
};