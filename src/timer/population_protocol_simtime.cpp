#include "population_protocol_simtime.h"
#include "utils.h"
#include <cassert>
#include <iostream>
#include <map>
#include <cmath>
#include <set>

PopulationProtocolSimtime::PopulationProtocolSimtime(uint32_t n, uint32_t k, bool is_trace): PopulationProtocol(n, k), rng_(n), is_trace_(is_trace) {
    assert(n > 2 && k > 2 && n % k == 0);
    up_to_date_agent_ = 0;
} 

auto PopulationProtocolSimtime::Check() -> bool {
    return final_agents_cnt_ == n_div_k_;
}

auto PopulationProtocolSimtime::Simulation_go() -> uint64_t {
    agent_sta_.clear();
    agent_sta_.push_back("INVALID");
    final_agents_cnt_ = 0;
    tot_switch_time_ = 0;
    unnull_time_ = 0;
    level_count_degeneracy_.clear();
    level_count_upgrade_.clear();
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
        if (is_trace_) {
            std::map<uint32_t, uint32_t>  mp;
            uint32_t up_to_date_agent = 0;
            for (int i  = 1; i <= n_; i++) {
                if (agent_sta_[i][0] == 'r') {
                    uint32_t tmp = 0;
                    for (int j = 1; j < agent_sta_[i].size(); j++) {
                        tmp = tmp * 10 + agent_sta_[i][j] - '0';
                    }
                    mp[tmp]++;
                }
            }
            for (int i = k_; i >= 1; i--) {
                if (mp[i] == n_div_k_) {
                    up_to_date_agent = i;
                    break;
                }
            }
            if (up_to_date_agent != up_to_date_agent_) {
                tot_switch_time_++;
                //std::cout << "switch:" << up_to_date_agent_ << " ---->" << up_to_date_agent << std::endl;
                if (up_to_date_agent_ < up_to_date_agent) {
                    level_count_upgrade_[up_to_date_agent_]++;
                }
                else {
                    level_count_degeneracy_[up_to_date_agent_]++;
                }
                up_to_date_agent_ = up_to_date_agent;
            }
        }
        time++;
    }
    return time;
}

void PopulationProtocolSimtime::SnapShot() {
    std::map<std::string, uint32_t> count;  
    for (int i = 1; i <= n_; i++) {
        count[agent_sta_[i]]++;
    }
    // * snapshot the current state *//
    for (auto &it: count) {
        std::cout << it.first << ": " << it.second << std::endl;
    }
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
       if(!ignore_result) std::cout << "[DEBUG: n = " << n_ << " k = " << 
            k_ << " cur_time = " << i <<  " num = " << time << "]" << std::endl; 
       tmp.push_back(time); 
    }
    avg = tot / times;
    for (auto &it: tmp) {
        std += (avg - it) * (avg - it);
    }
    std = sqrt(std);
    std::cout << "[INFO: n = " << n_ << " k = " << k_ << " avg = " << avg << " std = " << std << "]" << std::endl;
    std::cout << "[INFO: unnull time = " << unnull_time_ << "]" << std::endl;
    if (is_trace_) {
        std::cout << "total switch time = " << tot_switch_time_ << std::endl;
        uint64_t tot_upgrade_time = 0;
        for (auto &it: level_count_upgrade_) {
            tot_upgrade_time += it.second;
            std::cout << "r" << it.first << " upgrade: " << it.second << std::endl;
        }
        std::cout << "upgrade time :" << tot_upgrade_time << std::endl;
        uint64_t tot_degeneracy_time = 0;
        for (auto &it: level_count_degeneracy_) {
            tot_degeneracy_time += it.second;
            std::cout << "r" << it.first << " degeneracy: " << it.second << std::endl;
        }
        std::cout << "degeneracy time : " << tot_degeneracy_time << std::endl;
        std::cout << std::endl;
    }

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