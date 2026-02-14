#pragma once

#ifndef ARBITRARY_KDIVISION_WITH_BS_SIMPLE_PROTOCOL_H
#define ARBITRARY_KDIVISION_WITH_BS_SIMPLE_PROTOCOL_H

#include "protocol/BaseProtocol.h"
#include "state/ArbitraryKDivisionWithBSSimpleState.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <numeric>

template <>
struct ProtocolTraits<ArbitraryKDivisionWithBSSimpleProtocol> {
    using StateType = ArbitraryKDivisionWithBSSimpleState;
};


class ArbitraryKDivisionWithBSSimpleProtocol : public BaseProtocol<ArbitraryKDivisionWithBSSimpleProtocol> {
public:
    
    explicit ArbitraryKDivisionWithBSSimpleProtocol(int n, int divisions, std::vector<int> ratios): n(n), k(divisions), ratio(ratios) {
        if (k < 2) throw std::invalid_argument("k must be at least 2");
        if (k != ratio.size()) throw std::invalid_argument("k must be equal with ratio size");
        int g = std::accumulate(
            ratio.begin(), 
            ratio.end(), 
            0, 
            [](int a, int b) { return std::gcd(a, b); }
        );
        ratio_sum = 0;
        for (auto &it: ratio) {
            it /= g;
            ratio_sum += it;
        }
        if (n % ratio_sum) {
            throw std::invalid_argument("n must divide ratio sum"); 
        }
        expected_g_sig_siz = n * ratio.back() / ratio_sum / g;
        sum_ratio.push_back(0);
        for (int i = 1; i <= ratio.size(); i++) {
            sum_ratio.push_back(sum_ratio.back() + ratio[i - 1]);
        }
    }


   void interactImpl(Agent<ArbitraryKDivisionWithBSSimpleProtocol>& a, Agent<ArbitraryKDivisionWithBSSimpleProtocol>& b) {
        const auto* a_state = dynamic_cast<const ArbitraryKDivisionWithBSSimpleState*>(a.getState());
        const auto* b_state = dynamic_cast<const ArbitraryKDivisionWithBSSimpleState*>(b.getState());

        if (!a_state || !b_state) return;

        auto f = [&] (int x) {
            for (int j = 1; ; j++) {
               if (x > sum_ratio[j - 1] && x <= sum_ratio[j]) {
                    return j;
               } 
            }
        };


        int a_num_update = a_state->getNum();
        int b_num_update = b_state->getNum();

        if (!a_num_update && !b_num_update) return;

        if (a_state->isBs() && !b_num_update) {
            b_num_update = f(a_num_update);
            a_num_update = (a_num_update % ratio_sum) + 1; 
        }
        else if (b_state->isBs() && !a_num_update) {
            a_num_update = f(b_num_update);
            b_num_update = (b_num_update % ratio_sum) + 1;
        }
        else if (!a_num_update) {
            a_num_update = b_num_update;
            b_num_update = 0;
        }
        else if (!b_num_update) {
            b_num_update = a_num_update;
            a_num_update = 0;
        }

        a.setState(std::make_unique<ArbitraryKDivisionWithBSSimpleState>(a_state->isBs(), a_num_update));
        b.setState(std::make_unique<ArbitraryKDivisionWithBSSimpleState>(b_state->isBs(), b_num_update));
    }
    
    void initializeAgentsImpl(std::vector<Agent<ArbitraryKDivisionWithBSSimpleProtocol>>& agents) {
        // agents[0] alaways represents bs
        agents[0].setState(std::make_unique<ArbitraryKDivisionWithBSSimpleState>(true, 1));
        for (int i = 1; i < agents.size(); i++) {
            agents[i].setState(std::make_unique<ArbitraryKDivisionWithBSSimpleState>(false, 0));
        }
    }

    bool isConvergedImpl(const std::vector<Agent<ArbitraryKDivisionWithBSSimpleProtocol>>& agents) const {
        int counts = 0;

        for (const auto& agent : agents) {
            const auto* state = dynamic_cast<const ArbitraryKDivisionWithBSSimpleState*>(agent.getState());
            if (state && state->isBs()) {
                continue;
            }
            if (state && state->getNum()) {
                counts++;
            }
        }
        return counts == n;
    }


    std::unordered_map<RuleKey, long long, PairHash> getRuleCounts() {
        return rule_counts_;
    }

private:
    int k;
    int n;
    std::vector<int> ratio;
    std::vector<int> sum_ratio;
    int ratio_sum;
    int expected_g_sig_siz;
    std::unordered_map<RuleKey, long long, PairHash> rule_counts_;
};

class ArbitraryKDivisionWithBSSimpleProtocolFactory {
public:
    ArbitraryKDivisionWithBSSimpleProtocol create(int k, int n, std::vector<int>ratio) const {
        return ArbitraryKDivisionWithBSSimpleProtocol(n, k, ratio);
    }
};

#endif // ARBITRARY_KDIVISION_WITH_BS_PROTOCOL_H