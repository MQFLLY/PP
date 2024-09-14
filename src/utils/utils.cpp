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

void Utils::show_menu() {
    std::string input;
    while (1) {
        std::cout << "-------- SELECT AUTHOR -------" << std::endl;
        std::cout << "-------- 1: our work -------" << std::endl;
        std::cout << "-------- 2: Yasumi's work -------" << std::endl;
        std::cout << "-------- 3: exit -------" << std::endl;
        std::getline(std::cin, input);
        if (input != "1" && input != "2") {
            std::cout << "[warn] please input '1' or '2'" << std::endl;
        }
        else {
            break;
        }
    }
    if (input == "1") {
        std::cout << "------- our work -------" << std::endl;
        std::cout << "-------- SELECT TYPE -------" << std::endl;
        std::cout << "-------- 1: simulate time -------" << std::endl;
        std::cout << "------- 2: theoretical time -------" << std::endl;
        std::cout << "------- 3: get states number -------" << std::endl;
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
        std::cout << "-------- Yasumi's work -------" << std::endl;
        std::cout << "-------- simulate time -------" << std::endl;
        std::cout << "-------- input configs (n, k, iteration, is_trace, period) -------" << std::endl;
        int n, k, iteration, is_trace, period;
        std::cin >> n >> k >> iteration >> is_trace >> period;
    }
}