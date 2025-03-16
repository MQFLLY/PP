#ifndef KDIVISION_PROTOCOL_H
#define KDIVISION_PROTOCOL_H

#include "BaseProtocol.h"
#include "KDivisionState.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>

template <>
struct ProtocolTraits<KDivisionProtocol> {
    using StateType = KDivisionState;
};

class KDivisionProtocol : public BaseProtocol<KDivisionProtocol> {
    int k;

public:
    explicit KDivisionProtocol(int divisions) : k(divisions) {
        if (k < 2) throw std::invalid_argument("k must be at least 2");
    }

    void initializeAgents(std::vector<Agent<KDivisionProtocol>>& agents) override {
        for (auto& agent : agents) {
            agent.setState(std::make_unique<KDivisionState>("i1"));
        }
    }

    void interact(Agent<KDivisionProtocol>& a, Agent<KDivisionProtocol>& b) override {
        const auto* aState = dynamic_cast<const KDivisionState*>(a.getState());
        const auto* bState = dynamic_cast<const KDivisionState*>(b.getState());

        if (!aState || !bState) {
            throw std::logic_error("Invalid state type in KDivisionProtocol");
        }

        const auto& aValue = aState->getValue();
        const auto& bValue = bState->getValue();

        if(aValue == "i1" && bValue == "i1") {
            a.setState(std::make_unique<KDivisionState>("i2"));
            b.setState(std::make_unique<KDivisionState>("i2"));
            return;
        }
        if(aValue == "i2" && bValue == "i2") {
            a.setState(std::make_unique<KDivisionState>("i1"));
            b.setState(std::make_unique<KDivisionState>("i1"));
            return;
        }
        
        if((aValue == "i1" && bValue == "i2") || (aValue == "i2" && bValue == "i1")) {
            a.setState(std::make_unique<StateType>("g1"));
            b.setState(std::make_unique<StateType>("m1"));
            return;
        }
        if(aValue == "g1" && bValue == "m1") {
            a.setState(std::make_unique<StateType>("i1"));
            b.setState(std::make_unique<StateType>("i2"));
            return;
        }

        if((aValue == "i1" && bValue == "m1") || (aValue == "m1" && bValue == "i1")) {
            a.setState(std::make_unique<StateType>("g2"));
            b.setState(std::make_unique<StateType>("m2"));
            return;
        }
        if((aValue == "i2" && bValue == "m1") || (aValue == "m1" && bValue == "i2")) {
            a.setState(std::make_unique<StateType>("g2"));
            b.setState(std::make_unique<StateType>("m2"));
            return;
        }
        if(aValue == "g2" && bValue == "m2") {
            a.setState(std::make_unique<StateType>("i1"));
            b.setState(std::make_unique<StateType>("m1"));
            return;
        }

        for(int l = 2; l <= k-3; ++l) {
            std::string ml = "m" + std::to_string(l);
            std::string ml1 = "m" + std::to_string(l+1);
            std::string gl1 = "g" + std::to_string(l+1);

            if((aValue[0] == 'i' && bValue == ml) || (bValue[0] == 'i' && aValue == ml)) {
                a.setState(std::make_unique<StateType>(gl1));
                b.setState(std::make_unique<StateType>(ml1));
                return;
            }
            
            if((aValue == gl1 && bValue == ml1) || (bValue == gl1 && aValue == ml1)) {
                a.setState(std::make_unique<StateType>("i1"));
                b.setState(std::make_unique<StateType>(ml));
                return;
            }
        }

        std::string mk2 = "m" + std::to_string(k-2);
        if(aValue[0] == 'i' && bValue == mk2) {
            a.setState(std::make_unique<StateType>("g" + std::to_string(k-1)));
            b.setState(std::make_unique<StateType>("g" + std::to_string(k)));
            return;
        }
        if(bValue[0] == 'i' && aValue == mk2) {
            a.setState(std::make_unique<StateType>("g" + std::to_string(k)));
            b.setState(std::make_unique<StateType>("g" + std::to_string(k-1)));
            return;
        }
        
    }

    bool isConverged(const std::vector<Agent<KDivisionProtocol>>& agents) const override {
        std::unordered_map<std::string, int> counts;

        for (const auto& agent : agents) {
            const auto* state = dynamic_cast<const KDivisionState*>(agent.getState());
            if (state && state->getValue()[0] == 'g') {
                counts[state->getValue()]++;
            }
        }

        if (counts.size() != (size_t)k) return false;

        int expected = agents.size() / k;
        for (const auto& pair : counts) {
            if (pair.second != expected) return false;
        }
        return true;
    }
};

#endif