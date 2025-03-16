#include "StringState.h"

std::unique_ptr<State<StringProtocol>> StringState::clone() const {
    return std::make_unique<StringState>(value);
}

bool StringState::equals(const State<StringProtocol>* other) const {
    auto derived = dynamic_cast<const StringState*>(other);
    return derived && derived->value == value;
}