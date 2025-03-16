// BaseProtocol.h
#ifndef BASE_PROTOCOL_H
#define BASE_PROTOCOL_H

#include <vector>
#include "ProtocolTraits.h"
#include "Agent.h"

template <typename Derived>
class BaseProtocol {
public:
    using StateType = typename ProtocolTraits<Derived>::StateType;

    virtual void initializeAgents(std::vector<Agent<Derived>>& agents) = 0;
    virtual void interact(Agent<Derived>& a, Agent<Derived>& b) = 0;
    virtual bool isConverged(const std::vector<Agent<Derived>>& agents) const = 0;
};

#endif