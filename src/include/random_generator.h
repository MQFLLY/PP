#pragma once

#include <ctime>
#include <random>

class RandomGenerator {
 public:
    using interact_int_pair = std::pair<uint32_t, uint32_t>;
    RandomGenerator(int n) : n_(n), generator_(std::random_device()()), distribution_(1, n) {
        // generate a random number between [1, n] 
    }
    uint32_t generate() {
        return distribution_(generator_);
    }
    interact_int_pair gen() {
        uint32_t x = generate();
        uint32_t y = generate();
        while (x == y) {
            y = generate();
        }
        return std::make_pair(x, y);
    }
    void set_distribution(int n) {
        n_ = n;
        distribution_.reset();
        distribution_.param(std::uniform_int_distribution<uint32_t>::param_type(1, n_));
    }
 private:
    uint32_t n_;
    std::mt19937 generator_; 
    std::uniform_int_distribution<uint32_t> distribution_;
};