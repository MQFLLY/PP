#pragma once

#include "population_protocol.h"
#include "random_generator.h"

class PopulationProtocolSimtime: public PopulationProtocol {
 public:
   PopulationProtocolSimtime(uint32_t n, uint32_t k); 
    /** Initialize the rules*/
   virtual void Init() = 0;
    /** Check if converge*/
   auto Check() -> bool;
    /** go for one time*/
   auto Simulation_go() -> uint64_t;
    /** go for times time*/
   void Simulation(const uint32_t &times, const bool &ignore_result);
    /** go for all k with fixed n */
   void Simulation_varyingK(const uint32_t &times, const bool &ignore_result); 
    /** go for all n fixed k, n less than max_n */
   void Simulation_varyingN(const uint32_t &times, const bool &ignore_result, const uint32_t &max_n); 
 private:
    RandomGenerator rng_;
};