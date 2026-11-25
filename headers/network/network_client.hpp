#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include "wire_message.hpp"

#include <vector>

class INetworkClient {
public:
    virtual ~INetworkClient() = default;

    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual void update(double deltaTime) = 0;

    virtual bool sendMessage(const WireMessage& message) = 0;
    virtual std::vector<WireMessage> pollMessages() = 0;
};

#endif