#include "network/fake_network_client.hpp"

bool FakeNetworkClient::connect() {
    connected_ = true;
    return true;
}

void FakeNetworkClient::disconnect() {
    connected_ = false;
}

void FakeNetworkClient::update(double) {}

bool FakeNetworkClient::sendMessage(const WireMessage& message) {
    if (!connected_) {
        return false;
    }

    outgoing_.push_back(message);
    return true;
}

std::vector<WireMessage> FakeNetworkClient::pollMessages() {
    std::vector<WireMessage> messages;

    while (!incoming_.empty()) {
        messages.push_back(incoming_.front());
        incoming_.pop_front();
    }

    return messages;
}

void FakeNetworkClient::injectIncoming(const WireMessage& message) {
    incoming_.push_back(message);
}

std::vector<WireMessage> FakeNetworkClient::takeOutgoing() {
    std::vector<WireMessage> messages;
    messages.swap(outgoing_);
    return messages;
}

bool FakeNetworkClient::isConnected() const {
    return connected_;
}
