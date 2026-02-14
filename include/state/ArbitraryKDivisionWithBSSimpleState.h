#pragma once

#ifndef ARBITRARY_KDVISION_WITH_BS_SIMPLE_STATE_H
#define ARBITRARY_KDVISION_WITH_BS_SIMPLE_STATE_H

#include "state/State.h"
#include <string>
#include <memory>

class ArbitraryKDivisionWithBSSimpleProtocol;

class ArbitraryKDivisionWithBSSimpleState : public State<ArbitraryKDivisionWithBSSimpleProtocol> {
public:
    explicit ArbitraryKDivisionWithBSSimpleState(bool is_bs, int num): is_bs(is_bs), num(num) {}

    std::unique_ptr<State<ArbitraryKDivisionWithBSSimpleProtocol>> clone() const override {
        return std::make_unique<ArbitraryKDivisionWithBSSimpleState>(is_bs, num);
    }

    bool equals(const State<ArbitraryKDivisionWithBSSimpleProtocol>* other) const override {
        auto derived = dynamic_cast<const ArbitraryKDivisionWithBSSimpleState*>(other);
        return derived && derived->is_bs == is_bs && derived->num == num;
    }

    bool isBs() const { return is_bs; } 
    int getNum() const { return num; }

    virtual ~ArbitraryKDivisionWithBSSimpleState() = default;

private:
    bool is_bs;
    int num;
};

#endif // ARBITRARY_KDVISION_WITH_BS_SIMPLE_STATE_H