#include "protocol/StringProtocol.h"
#include "state/StringState.h"
#include <algorithm>

void StringProtocol::initializeAgentsImpl(std::vector<Agent<StringProtocol>>& agents) {
    for (auto& agent : agents) {
        agent.setState(std::make_unique<StringState>("init"));
    }
}

void StringProtocol::interactImpl(Agent<StringProtocol>& a, Agent<StringProtocol>& b) {
    const auto* aState = dynamic_cast<const StringState*>(a.getState());
    const auto* bState = dynamic_cast<const StringState*>(b.getState());

    if (!aState || !bState) {
        throw std::logic_error("Invalid state type in StringProtocol");
    }

    if (aState->getValue() == "init" && bState->getValue() == "init") {
        a.setState(std::make_unique<StringState>("ready"));
        b.setState(std::make_unique<StringState>("ready"));
    }
}

bool StringProtocol::isConvergedImpl(const std::vector<Agent<StringProtocol>>& agents) const {
    if (agents.empty()) return true;

    const auto* first = dynamic_cast<const StringState*>(agents[0].getState());
    if (!first) return false;

    const std::string target = first->getValue();
    return std::all_of(agents.begin(), agents.end(),
        [target](const Agent<StringProtocol>& a) {
            auto* s = dynamic_cast<const StringState*>(a.getState());
            return s && s->getValue() == target;
        });
}