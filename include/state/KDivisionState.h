#ifndef KDIVISION_STATE_H
#define KDIVISION_STATE_H

#include "state/State.h"
#include <string>
#include <memory>

class KDivisionProtocol;

class KDivisionState : public State<KDivisionProtocol> {
public:
    explicit KDivisionState(std::string s) : value(std::move(s)) {}

    std::unique_ptr<State<KDivisionProtocol>> clone() const override {
        return std::make_unique<KDivisionState>(value);
    }

    bool equals(const State<KDivisionProtocol>* other) const override {
        auto derived = dynamic_cast<const KDivisionState*>(other);
        return derived && derived->value == value;
    }

    std::string getValue() const { return value; }

    virtual ~KDivisionState() = default;
private:
    std::string value;
};

#endif