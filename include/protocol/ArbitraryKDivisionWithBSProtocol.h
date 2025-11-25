#pragma once

#ifndef ARBITRARY_KDIVISION_WITH_BS_PROTOCOL_H
#define ARBITRARY_KDIVISION_WITH_BS_PROTOCOL_H

#include "protocol/BaseProtocol.h"
#include "state/ArbitraryKDivisionWithBSState.h"
#include <vector>
#include <string>
#include <unordered_map>

template <>
struct ProtocolTraits<ArbitraryKDivisionWithBSProtocol> {
    using StateType = ArbitraryKDivisionWithBSState;
};


class ArbitraryKDivisionWithBSProtocol : public BaseProtocol<ArbitraryKDivisionWithBSProtocol> {
public:
    
    explicit ArbitraryKDivisionWithBSProtocol(int divisions): k(divisions) {
        if (k < 2) throw std::invalid_argument("k must be at least 2");
    }

   void interactImpl(Agent<ArbitraryKDivisionWithBSProtocol>& a, Agent<ArbitraryKDivisionWithBSProtocol>& b) {
        const auto* a_state = dynamic_cast<const ArbitraryKDivisionWithBSState*>(a.getState());
        const auto* b_state = dynamic_cast<const ArbitraryKDivisionWithBSState*>(b.getState());

        if (!a_state || !b_state) return;

        int a_dep = a_state->getDep();
        int a_dep_update = a_dep;
        int a_num = a_state->getNum();
        int a_num_update = a_num; 
        int b_dep = b_state->getDep();
        int b_dep_update = b_dep; 
        int b_num = b_state->getNum();
        int b_num_update = b_num;

        // std::cout << "[" << a_dep << ' ' << a_num << "] , [" << b_dep << ' ' << b_num << "]\n"; 

        if (a_state->isBs()) {
            if (b_dep != 1) {
                b_dep_update = 1;     
            }
            if (b_num == 0) {
                b_num_update = a_state->getNum();
                // bs increase one
                a.setState(std::make_unique<ArbitraryKDivisionWithBSState>(true, a_state->getNum() % k + 1, 0));
            }
            b.setState(std::make_unique<ArbitraryKDivisionWithBSState>(false, b_num_update, b_dep_update));
        }
        else if (b_state->isBs()) {
            if (a_dep != 1) {
                a_dep_update = 1;     
            }
            if (a_num == 0) {
                a_num_update = b_state->getNum();
                // bs increase one
                b.setState(std::make_unique<ArbitraryKDivisionWithBSState>(true, b_state->getNum() % k + 1, 0));
            }
            a.setState(std::make_unique<ArbitraryKDivisionWithBSState>(false, a_num_update, a_dep_update));
        }
        else {
            if (!a_dep && !b_dep) {
                return;
            }
            else if (!a_dep) {
                a_dep_update = b_dep + 1;
                a_num_update = b_num;
                b_num_update = 0; 
            }
            else if (!b_dep) {
                b_dep_update = a_dep + 1;
                b_num_update = a_num;
                a_num_update = 0;
            }
            else if (a_dep < b_dep) {
                b_dep_update  = a_dep + 1;
                if (a_num && !b_num) {
                    a_num_update = 0;
                    b_num_update = a_num;
                }
            }
            else if (b_dep < a_dep) {
                a_dep_update = b_dep + 1;
                if (b_num && !a_num) {
                    b_num_update = 0;
                    a_num_update = b_num;
                }
            }
            a.setState(std::make_unique<ArbitraryKDivisionWithBSState>(false, a_num_update, a_dep_update));
            b.setState(std::make_unique<ArbitraryKDivisionWithBSState>(false, b_num_update, b_dep_update));
        }
    }
    
    void initializeAgentsImpl(std::vector<Agent<ArbitraryKDivisionWithBSProtocol>>& agents) {
        // agents[0] alaways represents bs
        agents[0].setState(std::make_unique<ArbitraryKDivisionWithBSState>(true, 1, 0));
        for (int i = 1; i < agents.size(); i++) {
            agents[i].setState(std::make_unique<ArbitraryKDivisionWithBSState>(false, 0, 0));
        }
    }

    bool isConvergedImpl(const std::vector<Agent<ArbitraryKDivisionWithBSProtocol>>& agents) const {
        std::unordered_map<int, int> counts;

        std::cout << "checking..." << std::endl;
        for (const auto& agent : agents) {
            const auto* state = dynamic_cast<const ArbitraryKDivisionWithBSState*>(agent.getState());
            if (state && state->isBs()) {
                continue;
            }
            if (state) {
                counts[state->getNum()]++;
                std::cout << state->getNum() << ' ' << state->getDep() << '\n';
            }
        }

        if (counts.size() != (size_t)k) return false;

        int expected = (agents.size() - 1) / k;
        for (const auto& pair : counts) {
            if (pair.second != expected) return false;
        }
        return true;
    }

private:
    int k;
};

class ArbitraryKDivisionWithBSProtocolFactory {
public:
    ArbitraryKDivisionWithBSProtocol create(int k) const {
        return ArbitraryKDivisionWithBSProtocol(k);
    }
};

#endif // ARBITRARY_KDIVISION_WITH_BS_PROTOCOL_H