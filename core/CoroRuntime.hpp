//
// Created by divmone on 5/13/2026.
//

#pragma once

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include <string>

template<typename T>
int run(std::string_view name, T t) {
    int code = 0;

    boost::asio::io_context io;
    boost::asio::co_spawn(io, t(io), [name, &code](std::exception_ptr e) {
        if (e) {
            try {
                std::rethrow_exception(e);
            } catch (const std::exception &ex) {
                spdlog::error("[{}] Fatal: {}", name, ex.what());
            }
            code = 1;
        }
    });

    boost::asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([&io](auto, auto) {
        io.stop();
    });

    io.run();
    return code;
}

template<typename T>
boost::asio::awaitable<void> runCoro(std::string_view text, T t) {
    try {
        co_await t();
    } catch (const boost::system::system_error& e) {
        if (e.code() == boost::asio::error::eof ||
            e.code() == boost::asio::error::operation_aborted)
            spdlog::info("[{}] disconnected", text);
        else
            spdlog::error("[{}] io error: {}", text, e.what());
    } catch (const std::exception& e) {
        spdlog::error("[{}] error: {}", text, e.what());
    }
}
