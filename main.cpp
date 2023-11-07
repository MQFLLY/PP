#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

int main() {
    std::ofstream resultFile("result.txt");

    if (!resultFile.is_open()) {
        std::cerr << "Error: Failed to open result.txt" << std::endl;
        return 1;
    }

    std::string test1Command = "./time";  // Assuming test1 reads from stdin
    std::string test2Command = "./time2";  // Assuming test2 reads from stdin

    // Open the input file containing parameters
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Failed to open input.txt" << std::endl;
        return 1;
    }

    std::string param;

    // Execute test1
    FILE *test1Stream = popen(test1Command.c_str(), "w");
    if (test1Stream) {
        while (std::getline(inputFile, param)) {
            fprintf(test1Stream, "%s\n", param.c_str());
        }
        pclose(test1Stream);
    }

    // Execute test2
    inputFile.clear();  // Clear the end-of-file flag
    inputFile.seekg(0);  // Reset file pointer to the beginning
    FILE *test2Stream = popen(test2Command.c_str(), "w");
    if (test2Stream) {
        while (std::getline(inputFile, param)) {
            fprintf(test2Stream, "%s\n", param.c_str());
        }
        pclose(test2Stream);
    }

    // Close the input file
    inputFile.close();

    resultFile.close();
    return 0;
}
