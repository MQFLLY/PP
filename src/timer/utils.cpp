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