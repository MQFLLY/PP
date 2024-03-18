#pragma once

#include <string>
#include <utility>
#include <vector>
#include <map>

class PopulationProtocol {
 public:
    using interact_string_pair = std::pair<std::string, std::string>;
    using interact_int_pair = std::pair<uint32_t, uint32_t>;
    PopulationProtocol(uint32_t n, uint32_t k);
    virtual void Init() = 0;
    void Update(const int &x, const int &y);
 protected:
    /** The state of each agents */
    std::vector<std::string> agent_sta_;
    /** The transition rule */
    std::map<interact_string_pair, interact_string_pair> rule_;
    /** The number of agents */
    uint32_t n_;
    /** The number of groups */
    uint32_t k_;
    /** n / k , the number of agents in each group */
    uint32_t n_div_k_;
    /** The number of agents in final state */
    uint32_t final_agents_cnt_;
    /** The agent state in final state*/
    std::string final_agent_;
};