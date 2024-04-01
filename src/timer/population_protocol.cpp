#include "population_protocol.h"
#include "utils.h"

PopulationProtocol::PopulationProtocol(uint32_t n, uint32_t k): n_(n), k_(k) {
    n_div_k_ = n_ / k_;
}

void PopulationProtocol::Init() {

}

void PopulationProtocol::Update(const int &x, const int &y) {
    interact_string_pair interact_before = std::make_pair(agent_sta_[x], agent_sta_[y]);
    if(rule_.count(interact_before)) {
        auto interact_after = rule_[interact_before];
        if(interact_after.second == final_agent_) {
            final_agents_cnt_++;
        }
        agent_sta_[x] = interact_after.first;
        agent_sta_[y] = interact_after.second;
        unnull_time_++;
    }
}