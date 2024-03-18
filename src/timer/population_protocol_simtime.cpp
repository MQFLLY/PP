#include "population_protocol_simtime.h"
#include "utils.h"
#include <cassert>
#include <iostream>
#include <cmath>
#include <set>

PopulationProtocolSimtime::PopulationProtocolSimtime(uint32_t n, uint32_t k): PopulationProtocol(n, k), rng_(n) {
    assert(n > 2 && k > 2 && n % k == 0);
} 

auto PopulationProtocolSimtime::Check() -> bool {
    return final_agents_cnt_ == n_div_k_;
}

auto PopulationProtocolSimtime::Simulation_go() -> uint64_t {
    agent_sta_.clear();
    agent_sta_.push_back("INVALID");
    final_agents_cnt_ = 0;
    for (int i = 1; i <= n_; i++) {
        agent_sta_.push_back("i0");
    }
    uint64_t time = 0;
    while (true) {
        if (Check()) {
            return time;
        }
        auto p = rng_ .gen();
        Update(p.first, p.second);
        time++;
    }
    return time;
}

void PopulationProtocolSimtime::Simulation(const uint32_t &times, const bool &ignore_result) {
    final_agent_ = Utils::NumberToR(k_);
    n_div_k_ = n_ / k_;
    uint64_t avg = 0;
    uint64_t tot = 0;
    uint64_t std = 0;
    std::vector<uint64_t> tmp;
    for (int i = 0; i < times; i++) {
       uint64_t time = Simulation_go();
       tot += time;
       if(!ignore_result) std::cout << "n = " << n_ << " k = " << k_ << " i = " << i <<  " num = " << time << std::endl; 
       tmp.push_back(time); 
    }
    avg = tot / times;
    for (auto &it: tmp) {
        std += (avg - it) * (avg - it);
    }
    std = sqrt(std);
    std::cout << "n = " << n_ << " k = " << k_ << " avg = " << avg << " std = " << std << std::endl;

}

void PopulationProtocolSimtime::Simulation_varyingK(const uint32_t &times, const bool &ignore_result) {
    uint32_t orginK = k_;
    std::set<int> d;
    for (int i = 1; i * i <= n_; i++) {
        if (n_ % i) continue;
        if (i > 2) d.insert(i);
        if (n_ / i > 2) d.insert(n_ / i);
    }
    for (auto &k: d) {
       k_ = k;
       Init();
       Simulation(times, ignore_result); 
    }
    k_ = orginK;
}

void PopulationProtocolSimtime::Simulation_varyingN(const uint32_t &times, const bool &ignore_result, const uint32_t &max_n) {
    uint32_t originN = n_;
    Init(); 
    for (int i = k_; i <= max_n; i += k_) {
        n_ = i;
        rng_.set_distribution(n_);
        Simulation(times, ignore_result);
    }
    n_ = originN;
}