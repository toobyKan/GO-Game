#ifndef FAKE_NETWORK_CLIENT_H
#define FAKE_NETWORK_CLIENT_H

#include "network_client.hpp"
#include <deque>


class FakeNetworkClient : public INetworkClient {
public:

    bool connect() override;
    void disconnect() override;
    void update(double deltaTime) override;

    bool sendMessage(const WireMessage& message) override;
    std::vector<WireMessage> pollMessages() override;
    
    void injectIncoming(const WireMessage& message);
    std::vector<WireMessage> takeOutgoing();
    bool isConnected() const;
private: 
    bool connected_ = false;
    std::deque<WireMessage> incoming_;
    std::vector<WireMessage> outgoing_;
};

#endif
