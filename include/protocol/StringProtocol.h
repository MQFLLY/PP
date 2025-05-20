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
    void initializeAgentsImpl(std::vector<Agent<StringProtocol>>& agents);
    void interactImpl(Agent<StringProtocol>& a, Agent<StringProtocol>& b);
    bool isConvergedImpl(const std::vector<Agent<StringProtocol>>& agents) const;
};

#endif