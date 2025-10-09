#ifndef RATIO_KDIVISION_PARA_STATE_H
#define RATIO_KDIVISION_PARA_STATE_H

#include "state/State.h"
#include <string>
#include <memory>

class RatioKDivisionParaProtocol;

class RatioKDivisionParaState : public State<RatioKDivisionParaProtocol> {
public:
    explicit RatioKDivisionParaState(std::string s) : value(std::move(s)) {}

    std::unique_ptr<State<RatioKDivisionParaProtocol>> clone() const override {
        return std::make_unique<RatioKDivisionParaState>(value);
    }

    bool equals(const State<RatioKDivisionParaProtocol>* other) const override {
        auto derived = dynamic_cast<const RatioKDivisionParaState*>(other);
        return derived && derived->value == value;
    }

    std::string getValue() const { return value; }

    virtual ~RatioKDivisionParaState() = default;
private:
    std::string value;
};

#endif