#pragma once

#include "population_protocol_simtime.h"

class PopulationProtocolSimtime_OtherWork: public PopulationProtocolSimtime {
 public:
    PopulationProtocolSimtime_OtherWork(uint32_t n, uint32_t k, bool is_trace); 
    /** Initialize the rules*/
    virtual void Init() override;
};