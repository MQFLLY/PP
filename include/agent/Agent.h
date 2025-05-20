#ifndef AGENT_H
#define AGENT_H

#include <memory>
#include "state/State.h"

template <typename Protocol> class BaseProtocol;

template <typename Protocol>
class Agent {
public:
    void setState(std::unique_ptr<State<Protocol>> s) {
        state = std::move(s);
    }

    const State<Protocol>* getState() const {
        return state.get();
    }
private:
    std::unique_ptr<State<Protocol>> state;
};

#endif