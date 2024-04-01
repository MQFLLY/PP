#include "population_protocol.h"
#include "population_protocol_simtime.h"
#include "population_protocol_simtime_ourwork.h"
#include "population_protocol_simtime_otherwork.h"


// *example code *//
int main() {
    // PP by our work with condition n = 100, k = 10, trace the intermediate results 
    PopulationProtocolSimtime_OurWork pp1(100, 10, 1);
    // Remeber to Init before call Simulation
    pp1.Init();
    // Simulation 10 times and do not ignore each statistics with max n =  60 
    pp1.Simulation_varyingN(10, 0, 60);
}