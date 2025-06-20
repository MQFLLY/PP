#ifndef RATIO_KDIVISION_STATE_H
#define RATIO_KDIVISION_STATE_H

#include "state/State.h"
#include <string>
#include <memory>

class RatioKDivisionProtocol;

class RatioKDivisionState : public State<RatioKDivisionProtocol> {
public:
    explicit RatioKDivisionState(std::string s) : value(std::move(s)) {}

    std::unique_ptr<State<RatioKDivisionProtocol>> clone() const override {
        return std::make_unique<RatioKDivisionState>(value);
    }

    bool equals(const State<RatioKDivisionProtocol>* other) const override {
        auto derived = dynamic_cast<const RatioKDivisionState*>(other);
        return derived && derived->value == value;
    }

    std::string getValue() const { return value; }

    virtual ~RatioKDivisionState() = default;
private:
    std::string value;
};

#endif