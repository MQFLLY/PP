#pragma once

#include <string>
#include <utility>
#include <iostream>

class Utils {
 public:
    static std::string NumberToString(int n);
    static std::string NumberToR(int n);
    static std::string NumberToM(int n);
    static std::string NumberToD(int n);
    static std::string NumberToI(int n);
    static void show_menu();
    static void print_star(std::string str, int num_star = 7);
};