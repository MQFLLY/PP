#ifndef RULE_H
#define RULE_H

#include <unordered_map>

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

using RuleMap = std::unordered_map<RuleKey, RuleValue, PairHash>;
using RuleCountMap = std::unordered_map<RuleKey, long long, PairHash>;

#endif