#ifndef STATE_H
#define STATE_H

#include <memory>

template <typename Protocol>
class State {
public:
    virtual std::unique_ptr<State> clone() const = 0;
    virtual bool equals(const State* other) const = 0;
    virtual ~State() = default;
};

#endif // STATE_H