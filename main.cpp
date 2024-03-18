#include "population_protocol.h"
#include "population_protocol_simtime.h"
#include "population_protocol_simtime_ourwork.h"
#include "population_protocol_simtime_otherwork.h"

int main() {
    PopulationProtocolSimtime_OurWork pp1(16, 4);
    pp1.Simulation_varyingK(100, 1);
    pp1.Simulation_varyingN(100, 1, 20);
    PopulationProtocolSimtime_OtherWork pp2(16, 4);
    pp2.Simulation_varyingK(100, 1);
    pp2.Simulation_varyingN(100, 1, 20);
}