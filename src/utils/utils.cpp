#include "utils.h"
#include <algorithm>

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
        std::getline(std::cin, input);
        if (input != "1" && input != "2" && input != "3") {
            std::cout << "[warn] please input '1' or '2' or '3" << std::endl;
        }
        else {
            break;
        }
    }
    if (input == "1") {
        print_star("our work");
        print_star("SELECT TYPE");
        print_star("1: simulate time");
        print_star("2: theoretical time");
        print_star("3: get states number");
        std::getline(std::cin, input);
        if (input != "1" && input != "2" && input != "3") {
            std::cout << "[warn] please input '1' or '2' or '3'" << std::endl;
        }
        if (input == "1") {

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