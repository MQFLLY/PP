// include/StringState.h
#ifndef STRING_STATE_H
#define STRING_STATE_H

#include "state/State.h"

class StringProtocol;  

class StringState : public State<StringProtocol> {
    std::string value;

public:
    explicit StringState(std::string s) : value(std::move(s)) {}

    std::unique_ptr<State<StringProtocol>> clone() const override;
    bool equals(const State<StringProtocol>* other) const override;
    const std::string& getValue() const { return value; }
};

#endif