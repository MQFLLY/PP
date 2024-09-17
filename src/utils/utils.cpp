#include "utils.h"
#include <algorithm>
#include <memory>
#include "population_protocol_simtime_ourwork.h"
#include "population_protocol_simtime_otherwork.h"

std::string Utils::NumberToString(int n) {
    std::string tmp;
    do {
        tmp.push_back('0' + n % 10);
        n /= 10;
    } while(n);
    std::reverse(tmp.begin(), tmp.end());
    return tmp;
}

std::string Utils::NumberToR(int n) {
    return "r" + NumberToString(n);
}

std::string Utils::NumberToM(int n) {
    return "m" + NumberToString(n);
}

std::string Utils::NumberToD(int n) {
    return "d" + NumberToString(n);
}

std::string Utils::NumberToI(int n) {
    return "i" + NumberToString(n);
}

void Utils::print_star(std::string str, int num_star) {
   std::string star;
   for (int i = 0; i < num_star; i++) {
    star = star + "=";
   } 
   std::cout << star << " " << str << " " << star << std::endl;
}

void Utils::show_menu() {
    std::string input;
    while (1) {
        print_star("SELECT AUTHOR");
        print_star("1: our work");
        print_star("2: Yasumi's work");
        print_star("3: exit");
        std::cin >> input;
        if (input != "1" && input != "2" && input != "3") {
            std::cout << "[warn] please input '1' or '2' or '3" << std::endl;
        }
        else {
            if (input == "1") {
                print_star("our work");
                print_star("SELECT TYPE");
                print_star("1: simulate time");
                print_star("2: theoretical time");
                print_star("3: get states number");
                std::cin >> input;
                if (input != "1" && input != "2" && input != "3") {
                    std::cout << "[warn] please input '1' or '2' or '3'" << std::endl;
                    continue;
                }
                else if (input == "1") {
                    print_star("simulate time");
                    print_star("SELECT TYPE");
                    print_star("1: input with specific n, k, times (e.g. 1 8 4 100)");
                    print_star("2: varying k: input with specific n, times (e.g. 2 8 4 100)");
                    print_star("3: varying n: input with max n, specific k, times (e.g. 3 36 3 100)");
                    int n, k, times;
                    std::cin >> input >> n >> k >> times;
                    if (input != "1" && input != "2" && input != "3") {
                        std::cout << "[warn] please input '1' or '2' or '3'" << std::endl;
                        continue;
                    }
                    auto sim_our_work = std::make_shared<PopulationProtocolSimtime_OurWork>(n, k, false); 
                    sim_our_work->Init();
                    if (input == "1") {
                        sim_our_work->Simulation(times, false); 
                    }
                    else if (input == "2") {
                        sim_our_work->Simulation_varyingK(times, false);
                    }
                    else if (input == "3") {
                        sim_our_work->Simulation_varyingN(times, false, n);
                    }
                }
                else if (input == "2") {

                }
                else if (input == "3") {

                }
            }
            else if (input == "2") {
                print_star("Yasumi's work");
                print_star("simulate time");
                print_star("input configs (n, k, iteration, is_trace, period)");
                int n, k, iteration, is_trace, period;
                std::cin >> n >> k >> iteration >> is_trace >> period;
            }
            else {
                exit(0);
            }
        }
    }
}