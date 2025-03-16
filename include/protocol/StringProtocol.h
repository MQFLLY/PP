// include/StringProtocol.h
#ifndef STRING_PROTOCOL_H
#define STRING_PROTOCOL_H

#include "protocol/BaseProtocol.h"
#include "state/StringState.h" 

template <>
struct ProtocolTraits<StringProtocol> {
    using StateType = StringState;
};

class StringProtocol : public BaseProtocol<StringProtocol> {
public:
    void initializeAgents(std::vector<Agent<StringProtocol>>& agents) override;
    void interact(Agent<StringProtocol>& a, Agent<StringProtocol>& b) override;
    bool isConverged(const std::vector<Agent<StringProtocol>>& agents) const override;
};

#endif