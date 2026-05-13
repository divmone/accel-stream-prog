//
// Created by divmone on 5/10/2026.
//

#pragma once
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>

class Socket {
public:
    Socket(int domain, int type, int protocol) {
        sockfd = socket(domain, type, protocol);
        if (sockfd < 0) {
            throw std::runtime_error("Failed to create socket");
        }
    }
    ~Socket() {
        if (sockfd >= 0) {
            close(sockfd);
            sockfd = -1;
        }
    }

    explicit Socket(int fd): sockfd(fd) { }

    Socket(const Socket& ) = delete;
    Socket& operator=(const Socket& ) = delete;

    Socket(Socket &&other) noexcept
        : sockfd(other.sockfd) {
        other.sockfd = -1;
    }

    Socket & operator=(Socket &&other) noexcept {
        if (this == &other)
            return *this;

        if (sockfd > 0) {
            close(sockfd);
        }

        sockfd = other.sockfd;
        other.sockfd = -1;
        return *this;
    }

    int getFd() const {
        return sockfd;
    }

private:
    int sockfd;
};
