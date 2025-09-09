#ifndef RATIO_KDIVISION_PROTOCOL_H
#define RATIO_KDIVISION_PROTOCOL_H

#include "protocol/BaseProtocol.h"
#include "state/RatioKDivisionState.h"
#include <vector>
#include <string>
#include <numeric>
#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <spdlog/spdlog.h>

template <>
struct ProtocolTraits<RatioKDivisionProtocol> {
    using StateType = RatioKDivisionState;
};

class RatioKDivisionProtocol : public BaseProtocol<RatioKDivisionProtocol> {
/**
    f(x) = y iff \sum_{j = 1}^{y - 1} sig_j < x <= \sum_{j = 1}^y sig_j
    1. (i1,i1) → (i2,i2)
    2. (i2,i2) → (i1,i1)
    3. (i1,i2) → (g1,m1)
    4. (g1,m1) → (i1,i2)
    5. (il, mj) -> (g_f(j + 1), m_{j + 1}), 1 <= l <= 2, 1 <= j <= sig - 2
    6. (g_f(j + 1), m_j) -> (i1, m_j), 1 <= j <= sig - 2
    7. (i_l, m_{sig - 2}) -> (g_f(sig - 1), g_f(sig)), 1 <= l <= 2
**/
    void build_rule_cache() {
        add_symmetric_rule("i1", "i1", "i2", "i2");
        add_symmetric_rule("i2", "i2", "i1", "i1");
        add_symmetric_rule("i1", "i2", "g1", "m1");
        add_symmetric_rule("g1", "m1", "i1", "i2");
        
        int sig = std::accumulate(ratio.begin(), ratio.end(), 0);
        std::vector<int> sum_ratio(ratio.size() + 1);
        for (int i = 1; i <= ratio.size(); i++) {
            sum_ratio[i] = sum_ratio[i - 1] + ratio[i - 1];
        }
        auto f = [&] (int x) {
            for (int j = 1; ; j++) {
               if (x > sum_ratio[j - 1] && x <= sum_ratio[j]) {
                    return j;
               } 
            }
        };

        for(int j = 1; j <= sig - 2; ++j) {
            const std::string mj = "m" + std::to_string(j);
            const std::string g_f_j_plus_1 = "g" + std::to_string(f(j + 1));
            const std::string m_j_plus_1 = "m" + std::to_string(j + 1);
            
            add_symmetric_rule("i1", mj, g_f_j_plus_1, m_j_plus_1);
            add_symmetric_rule("i2", mj, g_f_j_plus_1, m_j_plus_1);
            
            add_symmetric_rule(g_f_j_plus_1, m_j_plus_1, "i1", mj);
        }

        const std::string mk2 = "m" + std::to_string(sig - 2);
        const std::string gk1 = "g" + std::to_string(f(sig - 1));
        const std::string gk = "g" + std::to_string(f(sig));
        add_symmetric_rule("i1", mk2, gk1, gk);
        add_symmetric_rule("i2", mk2, gk1, gk);
        /*
        std::cout << "====== Rule =======" << std::endl;
        for (auto [k, v]: rule_cache_) {
            auto [rule1, rule2] = k;
            auto [rule3, rule4] = v;
            std::cout << rule1 << ' ' << rule2 << ' ' << rule3 << ' ' << rule4 << std::endl;
        }
        */
    }

    void add_rule(const std::string& a, const std::string& b,
                  const std::string& new_a, const std::string& new_b) {
        rule_cache_[{a, b}] = {new_a, new_b};
    }

    void add_symmetric_rule(const std::string& a, const std::string& b,
                            const std::string& new_a, const std::string& new_b) {
        add_rule(a, b, new_a, new_b);
        add_rule(b, a, new_b, new_a);
    }

public:
    explicit RatioKDivisionProtocol(int n, int divisions, std::vector<int> ratios) : n(n), k(divisions), ratio(ratios) {
        if (k < 3) throw std::invalid_argument("k must be at least 3");
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
        build_rule_cache();
    }

    void interactImpl(Agent<RatioKDivisionProtocol>& a, Agent<RatioKDivisionProtocol>& b) {
        const auto* a_state = dynamic_cast<const RatioKDivisionState*>(a.getState());
        const auto* b_state = dynamic_cast<const RatioKDivisionState*>(b.getState());
        
        if(!a_state || !b_state) return;

        const auto& a_val = a_state->getValue();
        const auto& b_val = b_state->getValue();

        if(auto it = rule_cache_.find({a_val, b_val}); it != rule_cache_.end()) {
            a.setState(std::make_unique<RatioKDivisionState>(it->second.first));
            b.setState(std::make_unique<RatioKDivisionState>(it->second.second));
            rule_counts_[{a_val, b_val}]++;
        }
    }

    void printRuleCounts() const {
        long long tot_counts = 0;
        for (auto [rule, count]: rule_counts_) {
            tot_counts += count;
        } 
        spdlog::info("total rule counts: {}", tot_counts);
        for (auto [rule, count]: rule_counts_) {
            spdlog::info("rule: {},{}  count rate: {}", rule.first, rule.second, (double)count / tot_counts);
        }
    }

    std::unordered_map<RuleKey, long long, PairHash> getRuleCounts() {
        return rule_counts_;
    }

    void initializeAgentsImpl(std::vector<Agent<RatioKDivisionProtocol>>& agents) {
        for (auto& agent : agents) {
            agent.setState(std::make_unique<RatioKDivisionState>("i1"));
        }
    }

    bool isConvergedImpl(const std::vector<Agent<RatioKDivisionProtocol>>& agents) const {
        int count = 0;
        static int total_count = 0;
        for (const auto& agent : agents) {
            const auto* state = dynamic_cast<const RatioKDivisionState*>(agent.getState());
            /*
            if (total_count < 300) {
                std::cout << state->getValue() << std::endl;
            }
            */
            if (state && state->getValue()[0] == 'g') {
                count++;
            }
        }
        total_count++;
       return count == n;
    }

private:
    int k;
    int n;
    std::vector<int> ratio;
    int ratio_sum;
    int expected_g_sig_siz; 
    
    std::unordered_map<RuleKey, RuleValue, PairHash> rule_cache_;
    std::unordered_map<RuleKey, long long, PairHash> rule_counts_;

};

class RatioKDivisionProtocolFactory {
public:
    RatioKDivisionProtocol create(int k, int n, std::vector<int>ratios) const {
        return RatioKDivisionProtocol(n, k, ratios);
    }
};

#endif