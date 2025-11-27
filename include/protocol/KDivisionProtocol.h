#ifndef KDIVISION_PROTOCOL_H
#define KDIVISION_PROTOCOL_H

#include "protocol/BaseProtocol.h"
#include "state/KDivisionState.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <utility>

template <>
struct ProtocolTraits<KDivisionProtocol> {
    using StateType = KDivisionState;
};

class KDivisionProtocol : public BaseProtocol<KDivisionProtocol> {
/**
    1. (i1,i1) → (i2,i2)
    2. (i2,i2) → (i1,i1)
    3. (i1,i2) → (g1,m1)
    4. (g1,m1) → (i1,i2)
    5. (i1,m1) → (g2,m2)
    6. (i2,m1) → (g2,m2)
    7. (g2,m2) → (i1,m1)
    8. (ij, ml) → (gl+1,ml+1) (1 ≤ j ≤ 2,2 ≤ l ≤ k−3)
    9. (gl+1,ml+1) → (i1,ml) (2 ≤ l ≤ k −3)
    10. (ij,mk−2) → (gk−1,gk) (1 ≤ j ≤ 2)

**/
    void build_rule_cache() {
        add_symmetric_rule("i1", "i1", "i2", "i2");
        add_symmetric_rule("i2", "i2", "i1", "i1");
        add_symmetric_rule("i1", "i2", "g1", "m1");
        add_symmetric_rule("g1", "m1", "i1", "i2");
        add_symmetric_rule("i1", "m1", "g2", "m2");
        add_symmetric_rule("i2", "m1", "g2", "m2");
        add_symmetric_rule("g2", "m2", "i1", "m1");

        for(int l = 2; l <= k - 3; ++l) {
            const std::string ml = "m" + std::to_string(l);
            const std::string gl = "g" + std::to_string(l+1);
            const std::string ml1 = "m" + std::to_string(l+1);
            
            add_symmetric_rule("i1", ml, gl, ml1);
            add_symmetric_rule("i2", ml, gl, ml1);
            
            add_symmetric_rule(gl, ml1, "i1", ml);
        }

        const std::string mk2 = "m" + std::to_string(k-2);
        const std::string gk1 = "g" + std::to_string(k-1);
        const std::string gk = "g" + std::to_string(k);
        add_symmetric_rule("i1", mk2, gk1, gk);
        add_symmetric_rule("i2", mk2, gk1, gk);
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
    explicit KDivisionProtocol(int divisions) : k(divisions) {
        if(k < 3) throw std::invalid_argument("k must be at least 3");
        build_rule_cache();
    }

    /**
        1. (i1,i1) → (i2,i2)
        2. (i2,i2) → (i1,i1)
        3. (i1,i2) → (g1,m1)
        4. (g1,m1) → (i1,i2)
        5. (i1,m1) → (g2,m2)
        6. (i2,m1) → (g2,m2)
        7. (g2,m2) → (i1,m1)
        8. (ij, ml) → (gl+1,ml+1) (1 ≤ j ≤ 2,2 ≤ l ≤ k−3)
        9. (gl+1,ml+1) → (i1,ml) (2 ≤ l ≤ k −3)
        10. (ij,mk−2) → (gk−1,gk) (1 ≤ j ≤ 2)
     */

    void interactImpl(Agent<KDivisionProtocol>& a, Agent<KDivisionProtocol>& b) {
        const auto* a_state = dynamic_cast<const KDivisionState*>(a.getState());
        const auto* b_state = dynamic_cast<const KDivisionState*>(b.getState());
        
        if(!a_state || !b_state) return;

        const auto& a_val = a_state->getValue();
        const auto& b_val = b_state->getValue();

        // match rule
        if(auto it = rule_cache_.find({a_val, b_val}); it != rule_cache_.end()) {
            a.setState(std::make_unique<KDivisionState>(it->second.first));
            b.setState(std::make_unique<KDivisionState>(it->second.second));
        }
        else {
            a.setState(std::make_unique<KDivisionState>(b_val));
            b.setState(std::make_unique<KDivisionState>(a_val));
        }
    }

    void initializeAgentsImpl(std::vector<Agent<KDivisionProtocol>>& agents) {
        for (auto& agent : agents) {
            agent.setState(std::make_unique<KDivisionState>("i1"));
        }
    }

    bool isConvergedImpl(const std::vector<Agent<KDivisionProtocol>>& agents) const {
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

private:
    int k;
    using RuleKey = std::pair<std::string, std::string>;
    using RuleValue = std::pair<std::string, std::string>;


    struct PairHash {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1,T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };
    
    std::unordered_map<RuleKey, RuleValue, PairHash> rule_cache_;
};

class KDivisionProtocolFactory {
public:
    KDivisionProtocol create(int k) const {
        return KDivisionProtocol(k);
    }
};

#endif