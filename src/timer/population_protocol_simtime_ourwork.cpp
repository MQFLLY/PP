#include "population_protocol_simtime_ourwork.h"
#include "utils.h"

PopulationProtocolSimtime_OurWork::PopulationProtocolSimtime_OurWork(uint32_t n, uint32_t k, bool is_trace): PopulationProtocolSimtime(n, k, is_trace) {}

void PopulationProtocolSimtime_OurWork::Init() {
    rule_.clear();
    rule_[std::make_pair("i0","i0")] = std::make_pair("i1","i1");
    rule_[std::make_pair("i1","i1")] = std::make_pair("i0","i0");
    rule_[std::make_pair("i0","i1")] = std::make_pair("r1","m1");
    rule_[std::make_pair("i1","i0")] = std::make_pair("r1","m1");
    rule_[std::make_pair("r1","m1")] = std::make_pair("i0","i1");
    rule_[std::make_pair("m1","r1")] = std::make_pair("i0","i1");
    for(int i = 2;i <= k_ - 2;i++) {
    	rule_[std::make_pair("i0",Utils::NumberToM(i - 1))] = std::make_pair(Utils::NumberToM(i),Utils::NumberToR(i));
    	rule_[std::make_pair(Utils::NumberToM(i - 1),"i0")] = std::make_pair(Utils::NumberToM(i),Utils::NumberToR(i));
    	rule_[std::make_pair("i1",Utils::NumberToM(i - 1))] = std::make_pair(Utils::NumberToM(i),Utils::NumberToR(i));
    	rule_[std::make_pair(Utils::NumberToM(i - 1),"i1")] = std::make_pair(Utils::NumberToM(i),Utils::NumberToR(i));
		rule_[std::make_pair(Utils::NumberToM(i),Utils::NumberToR(i))] = std::make_pair("i0",Utils::NumberToM(i - 1));
		rule_[std::make_pair(Utils::NumberToR(i),Utils::NumberToM(i))] = std::make_pair("i0",Utils::NumberToM(i - 1));
    }
    rule_[std::make_pair("i0",Utils::NumberToM(k_ - 2))] = std::make_pair(Utils::NumberToR(k_ - 1),Utils::NumberToR(k_));
    rule_[std::make_pair(Utils::NumberToM(k_ - 2),"i0")] = std::make_pair(Utils::NumberToR(k_ - 1),Utils::NumberToR(k_));
    rule_[std::make_pair("i1",Utils::NumberToM(k_ - 2))] = std::make_pair(Utils::NumberToR(k_ - 1),Utils::NumberToR(k_));
    rule_[std::make_pair(Utils::NumberToM(k_ - 2),"i1")] = std::make_pair(Utils::NumberToR(k_ - 1),Utils::NumberToR(k_));
}