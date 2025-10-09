#ifndef RATIO_KDIVISION_PARA_PROTOCOL_H
#define RATIO_KDIVISION_PARA_PROTOCOL_H

#include "protocol/BaseProtocol.h"
#include "state/RatioKDivisionParaState.h"
#include <vector>
#include <string>
#include <numeric>
#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <random>
#include <spdlog/spdlog.h>

template <>
struct ProtocolTraits<RatioKDivisionParaProtocol> {
    using StateType = RatioKDivisionParaState;
};

class RatioKDivisionParaProtocol : public BaseProtocol<RatioKDivisionParaProtocol> {
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
        add_symmetric_rule("i1", "i2", "g1", "m1,1");
        
        for (int i = 1; i <= k; i++) {
            auto m_i = "m" + std::to_string(i) + ",";
            auto m_i_minus_1 = "m" + std::to_string(i - 1 >= 1 ? i - 1 : k) + ",";
            auto g_i = "g" + std::to_string(i);
            int tot_i = n * ratio[i - 1] / ratio_sum;
            int tot_i_minus_1 = n * ratio[(i - 2) >= 0 ? i - 2 : k - 1] / ratio_sum;
            if (tot_i == 1) {
                add_symmetric_rule(m_i_minus_1 + std::to_string(tot_i_minus_1), "i1", g_i, m_i + std::to_string(tot_i));
                add_symmetric_rule(g_i, m_i + std::to_string(tot_i), m_i_minus_1 + std::to_string(tot_i_minus_1), "i1"); 
                continue;
            }
            if (i == 1 && tot_i == 2) {
                add_symmetric_rule(m_i + std::to_string(2), g_i, "i1", "i2");
            }
            else {
                add_symmetric_rule(m_i + std::to_string(1), g_i, "i1", "i2");
            }
            for (int j = 1; j <= tot_i - 2; j++) {
                add_symmetric_rule("i1", m_i + std::to_string(j),
                                    g_i, m_i + std::to_string(j + 1));
                add_symmetric_rule(g_i, m_i + std::to_string(j + 1),
                                    "i1", m_i + std::to_string(j));
                add_symmetric_rule("i2", m_i + std::to_string(j),
                                    g_i, m_i + std::to_string(j + 1));
                add_symmetric_rule(g_i, m_i + std::to_string(j + 1),
                                    "i2", m_i + std::to_string(j));
            }
            if (i == 1 && tot_i - 2) {
                add_symmetric_rule("i1", m_i + std::to_string(tot_i - 2),
                                    g_i, m_i + std::to_string(tot_i));
                add_symmetric_rule(g_i, m_i + std::to_string(tot_i),
                                    "i1", m_i + std::to_string(tot_i - 2));
                add_symmetric_rule("i2", m_i + std::to_string(tot_i - 2),
                                    g_i, m_i + std::to_string(tot_i));
                add_symmetric_rule(g_i, m_i + std::to_string(tot_i),
                                    "i2", m_i + std::to_string(tot_i - 2));
            }
            if (i == k) {
                add_symmetric_rule(m_i_minus_1 + std::to_string(tot_i_minus_1), m_i + std::to_string(tot_i - 1), g_i, "g1");
            }
            else if (i > 1) {
                std::string prev = m_i_minus_1 + std::to_string(tot_i_minus_1);
                if (tot_i_minus_1 == 1 && i - 1 == 1) {
                    prev = "i1";
                }
                add_symmetric_rule(prev, m_i + std::to_string(tot_i - 1), g_i, m_i + std::to_string(tot_i));
                add_symmetric_rule(g_i, m_i + std::to_string(tot_i), prev, m_i + std::to_string(tot_i - 1)); 
            }
        }
        
        std::cout << "====== Rule =======" << std::endl;
        for (auto [k, v]: rule_cache_) {
            auto [rule1, rule2] = k;
            auto [rule3, rule4] = v;
            std::cout << rule1 << ' ' << rule2 << ' ' << rule3 << ' ' << rule4 << std::endl;
        }
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
    explicit RatioKDivisionParaProtocol(int n, int divisions, std::vector<int> ratios) : n(n), k(divisions), ratio(ratios), gen(divisions) {
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

    void interactImpl(Agent<RatioKDivisionParaProtocol>& a, Agent<RatioKDivisionParaProtocol>& b) {
        const auto* a_state = dynamic_cast<const RatioKDivisionParaState*>(a.getState());
        const auto* b_state = dynamic_cast<const RatioKDivisionParaState*>(b.getState());
        
        if(!a_state || !b_state) return;

        const auto& a_val = a_state->getValue();
        const auto& b_val = b_state->getValue();
       
        rule_counts_[{a_val, b_val}]++;
        
        if ((a_val == "i1" && b_val == "i2") || (a_val == "i2" && b_val == "i1")) {
            int target = gen.gen();
            while (true) {
                if (ratio[target - 1]  * n / ratio_sum == 1) {
                    target = gen.gen();
                    continue;
                } else {
                    a.setState(std::make_unique<RatioKDivisionParaState>("g" + std::to_string(target)));
                    if (target == 1 && ratio[target - 1] * n / ratio_sum == 2)
                        b.setState(std::make_unique<RatioKDivisionParaState>("m" + std::to_string(target) + ",2"));
                    else 
                        b.setState(std::make_unique<RatioKDivisionParaState>("m" + std::to_string(target) + ",1"));
                    break;
                }
            }
            return;
        }

        if (auto it = rule_cache_.find({a_val, b_val}); it != rule_cache_.end()) {
            a.setState(std::make_unique<RatioKDivisionParaState>(it->second.first));
            b.setState(std::make_unique<RatioKDivisionParaState>(it->second.second));
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

    void initializeAgentsImpl(std::vector<Agent<RatioKDivisionParaProtocol>>& agents) {
        for (auto& agent : agents) {
            agent.setState(std::make_unique<RatioKDivisionParaState>("i1"));
        }
    }

    bool isConvergedImpl(const std::vector<Agent<RatioKDivisionParaProtocol>>& agents) const {
        int count = 0;
        static int total_count = 0;
        const int LOG_TIME = 10000000; 
        if (0 == total_count % LOG_TIME) {
            std::cout << "LOGGING..." << std::endl;
        } 
        for (const auto& agent : agents) {
            const auto* state = dynamic_cast<const RatioKDivisionParaState*>(agent.getState());
            if (0 == total_count % LOG_TIME) {
                std::cout << state->getValue() << std::endl;
            }
            if (state && state->getValue()[0] == 'g') {
                count++;
            }
        }
        total_count++;
        if (count == n) {
            std::cout << "ENDING..." << std::endl;
            for (const auto& agent: agents) {
                const auto* state = dynamic_cast<const RatioKDivisionParaState*>(agent.getState());
                std::cout << state->getValue() << std::endl;
            }
        }
       return count == n;
    }

private:

    class RandomNumberGenerator {
    private:
        int k;
        std::mt19937 rng;
        std::uniform_int_distribution<int> dist;
        
    public:
        RandomNumberGenerator(int k) : k(k), dist(1, k) {
            std::random_device rd;
            rng.seed(rd());
        }
        
        int gen() {
            return dist(rng);
        }
    };
    RandomNumberGenerator gen;
    int k;
    int n;
    std::vector<int> ratio;
    int ratio_sum;
    int expected_g_sig_siz; 
    std::unordered_map<RuleKey, RuleValue, PairHash> rule_cache_;
    std::unordered_map<RuleKey, long long, PairHash> rule_counts_;

};

class RatioKDivisionParaProtocolFactory {
public:
    RatioKDivisionParaProtocol create(int k, int n, std::vector<int>ratios) const {
        return RatioKDivisionParaProtocol(n, k, ratios);
    }
};

#endif