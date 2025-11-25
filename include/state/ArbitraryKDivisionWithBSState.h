#pragma once

#ifndef ARBITRARY_KDVISION_WITH_BS_STATE_H
#define ARBITRARY_KDVISION_WITH_BS_STATE_H

#include "state/State.h"
#include <string>
#include <memory>

class ArbitraryKDivisionWithBSProtocol;

class ArbitraryKDivisionWithBSState : public State<ArbitraryKDivisionWithBSProtocol> {
public:
    explicit ArbitraryKDivisionWithBSState(bool is_bs, int num, int dep): is_bs(is_bs), num(num), dep(dep) {}

    std::unique_ptr<State<ArbitraryKDivisionWithBSProtocol>> clone() const override {
        return std::make_unique<ArbitraryKDivisionWithBSState>(is_bs, num, dep);
    }

    bool equals(const State<ArbitraryKDivisionWithBSProtocol>* other) const override {
        auto derived = dynamic_cast<const ArbitraryKDivisionWithBSState*>(other);
        return derived && derived->is_bs == is_bs && derived->num == num && derived->dep == dep;
    }

    bool isBs() const { return is_bs; } 
    int getNum() const { return num; }
    int getDep() const { return dep; }

    virtual ~ArbitraryKDivisionWithBSState() = default;

private:
    bool is_bs;
    int num;
    int dep;
};

#endif // ARBITRARY_KDVISION_WITH_BS_STATE_H