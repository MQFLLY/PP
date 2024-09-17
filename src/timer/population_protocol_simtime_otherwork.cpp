#include "population_protocol_simtime_otherwork.h"
#include "utils.h"

PopulationProtocolSimtime_OtherWork::PopulationProtocolSimtime_OtherWork(uint32_t n, uint32_t k, bool is_trace): PopulationProtocolSimtime(n, k, is_trace) {}

void PopulationProtocolSimtime_OtherWork::Init() {

    rule_.clear();
	//1
	rule_[std::make_pair("i0", "i0")] = std::make_pair("i1", "i1");
	//2
	rule_[std::make_pair("i1", "i1")] = std::make_pair("i0", "i0");
	//5
	rule_[std::make_pair("i0","i1")] = std::make_pair("r1","m2");
	//3
	for(int i = 1;i <= k_ - 2; i++) {
		rule_[std::make_pair(Utils::NumberToD(i), "i0")] = std::make_pair(Utils::NumberToD(i), "i1");
		rule_[std::make_pair(Utils::NumberToD(i), "i1")] = std::make_pair(Utils::NumberToD(i), "i0");
	}
	//4
	for(int i = 1;i <= k_; i++) {
		rule_[std::make_pair(Utils::NumberToR(i), "i0")] = std::make_pair(Utils::NumberToR(i), "i1");
		rule_[std::make_pair(Utils::NumberToR(i), "i1")] = std::make_pair(Utils::NumberToR(i), "i0");
	}
	//6
	for(int i = 2;i <= k_ - 2; i++) {
		rule_[std::make_pair("i0", Utils::NumberToM(i))] = std::make_pair(Utils::NumberToR(i), Utils::NumberToM(i + 1));
		rule_[std::make_pair("i1", Utils::NumberToM(i))] = std::make_pair(Utils::NumberToR(i), Utils::NumberToM(i + 1));
	}
	//7
	rule_[std::make_pair("i0", Utils::NumberToM(k_ - 1))] = std::make_pair(Utils::NumberToR(k_ - 1), Utils::NumberToR(k_));
	rule_[std::make_pair("i1", Utils::NumberToM(k_ - 1))] = std::make_pair(Utils::NumberToR(k_ - 1), Utils::NumberToR(k_));
	//8
	for(int i = 2; i <= k_ - 1; i++) {
		for(int j = 2; j <= k_ - 1; j++) {
			rule_[std::make_pair(Utils::NumberToM(i),Utils::NumberToM(j))] = std::make_pair(Utils::NumberToD(i - 1), Utils::NumberToD(j - 1));
		}
	}
	//9
	for(int i = 2; i <= k_ - 2; i++) {
		rule_[std::make_pair(Utils::NumberToD(i), Utils::NumberToR(i))] = std::make_pair(Utils::NumberToD(i - 1), "i0");	
	}
	//10
	rule_[std::make_pair("d1", "r1")] = std::make_pair("i0", "i0");
    std::cout << "[INFO: RULE INITIALIZED]" << std::endl;
}