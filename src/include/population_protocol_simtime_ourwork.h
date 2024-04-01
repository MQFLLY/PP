#pragma once

#include "population_protocol_simtime.h"

class PopulationProtocolSimtime_OurWork: public PopulationProtocolSimtime {
 public:
    PopulationProtocolSimtime_OurWork(uint32_t n, uint32_t k, bool is_trace); 
    /** Initialize the rules*/
    virtual void Init() override;
};