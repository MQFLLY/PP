// BaseProtocol.h
#ifndef BASE_PROTOCOL_H
#define BASE_PROTOCOL_H

#include <vector>
#include <iostream>
#include "protocol/ProtocolTraits.h"
#include "agent/Agent.h"

template <typename Derived>
class BaseProtocol {
public:
    using StateType = typename ProtocolTraits<Derived>::StateType;

    void initializeAgents(std::vector<Agent<Derived>>& agents) {
        static_cast<Derived*>(this)->initializeAgentsImpl(agents);
    }

    void interact(Agent<Derived>& a, Agent<Derived>& b) {
        static_cast<Derived*>(this)->interactImpl(a, b);
    }

    bool isConverged(const std::vector<Agent<Derived>>& agents) const {
        return static_cast<const Derived*>(this)->isConvergedImpl(agents);
    }
};

#endif