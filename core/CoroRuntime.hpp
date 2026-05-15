//
// Created by divmone on 5/13/2026.
//

#pragma once

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <TcpClient.hpp>

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

template <typename Handler>
boost::asio::awaitable<void> runTcpClient(
    std::string_view text,
    boost::asio::io_context& io,
    std::string_view host,
    int port,
    Handler h)
{
    spdlog::info("{} starting", text);

    auto ex = co_await boost::asio::this_coro::executor;
    constexpr auto RECONNECT_DELAY = std::chrono::seconds(1);

    while (true) {
        try {
            auto conn = co_await TcpClient::connect(io, host, port);
            spdlog::info("[{}] processing loop started", text);
            while (true) {
               co_await h(conn);
            }
        } catch (const boost::system::system_error &e) {
            spdlog::warn("[{}] disconnected: {}", text, e.what());
        } catch (const std::exception &e) {
            spdlog::error("[{}] error: {}", text, e.what());
        }

        spdlog::info("[{}] reconnecting in {}ms", text,
                     std::chrono::duration_cast<std::chrono::milliseconds>(RECONNECT_DELAY).count());
        boost::asio::steady_timer timer(ex, RECONNECT_DELAY);
        co_await timer.async_wait(boost::asio::use_awaitable);
    }
}

