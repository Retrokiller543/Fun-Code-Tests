#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <iomanip>
#include <sstream>

#include "/home/tosic-killer/Dev/Fun-Code-Tests/hashmap-lookup/include/progresscpp/ProgressBar.hpp"

std::string generate_random_hex_string(size_t length);

int main() {
    std::ofstream outfile("signatures.db");
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist_hex_length(10, 200);

    int count = 10000;

    if (outfile.is_open()) {
        progresscpp::ProgressBar progressBar(count, 70); // Initialize the progress bar with 10,000,000 steps and width of 70

        for (int i = 1; i <= count; ++i) {
            std::string key = "TestRandom." + std::to_string(i);
            std::string value = generate_random_hex_string(dist_hex_length(gen));
            outfile << key << "=" << value << "\n";

            ++progressBar; // update progress bar
            progressBar.display(); // display progress bar
        }

        progressBar.done(); // Finish the progress bar

        outfile.close();
    } else {
        std::cerr << "Unable to open file: signatures.db" << std::endl;
    }

    return 0;
}

std::string generate_random_hex_string(size_t length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dist_hex(0, 65535);

    std::stringstream ss;
    ss.str().reserve(length);

    for (size_t i = 0; i < length; i += 4) {
        uint16_t val = dist_hex(gen);
        ss << std::hex << std::setw(4) << std::setfill('0') << val;
    }

    return ss.str().substr(0, length);
}
