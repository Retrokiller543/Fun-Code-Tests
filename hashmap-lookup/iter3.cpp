#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "/home/tosic-killer/Dev/Fun-Code-Tests/hashmap-lookup/include/progresscpp/ProgressBar.hpp"


std::pair<std::unordered_map<std::string, std::string>, std::unordered_map<std::string, std::string>> read_data(const std::string& filename);
std::vector<std::string> pick_random_values(const std::unordered_map<std::string, std::string>& data, size_t count);

std::optional<std::string> find_key_by_value(const std::unordered_map<std::string, std::string>& value_to_key, const std::string& value_to_find);

std::optional<std::string> find_key_by_linear_search(const std::unordered_map<std::string, std::string>& value_to_key, const std::string& value);
std::optional<std::string> find_key_by_binary_search(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::string& value);
std::optional<std::string> find_key_by_hash_table_search(const std::unordered_map<std::string, std::string>& value_to_key, const std::string& value);
std::optional<std::string> find_key_by_ternary_search(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::string& value, int left, int right);
std::optional<std::string> find_key_by_exponential_search(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::string& value);

template <typename Func, typename Container>
void run_tests(const std::string& test_name, Func search_func, const Container& container, const std::vector<std::string>& test_values, std::ofstream& log_file);

void run_linear_search_tests(const std::unordered_map<std::string, std::string>& value_to_key, const std::vector<std::string>& test_values, std::ofstream& log_file);
void run_binary_search_tests(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::vector<std::string>& test_values, std::ofstream& log_file);
void run_hash_table_search_tests(const std::unordered_map<std::string, std::string>& value_to_key, const std::vector<std::string>& test_values, std::ofstream& log_file);
void run_ternary_search_tests(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::vector<std::string>& test_values, std::ofstream& log_file);
void run_exponential_search_tests(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::vector<std::string>& test_values, std::ofstream& log_file);

int main() {
    std::string filename = "signatures.db";
    auto [hexData, value_to_key] = read_data(filename);

    // Create a sorted vector of key-value pairs for binary search
    std::vector<std::pair<std::string, std::string>> sorted_data(value_to_key.begin(), value_to_key.end());
std::sort(sorted_data.begin(), sorted_data.end(), [](const auto& a, const auto& b) {
return a.second < b.second;
});
// Prepare a list of hex values to test
std::vector<std::string> test_values = pick_random_values(hexData, 100);

// Add some hex values that are not in the data
test_values.push_back("255044462d312e340a332030206f626a203c3c0a2f4c656e677468203139");
test_values.push_back("feedface00000012000000000000000200000016000009b00000009500000001000000385f5f504147455a45524f0000");
test_values.push_back("93c20c003c5f000080420cec81820000");
test_values.push_back("7c9d23787cbe2b78480001b13c5f0000");
test_values.push_back("9421ffa0429f00057fe802a67c7c1b78");

// Open the log file for writing or appending
std::ofstream log_file("output.log", std::ios_base::app);

// Write a header to the log file
log_file << "==============================" << std::endl;
log_file << "Results of test run on " << __DATE__ << " at " << __TIME__ << std::endl;
log_file << "==============================" << std::endl << std::endl;

// Run linear search tests and append output to the log file
run_linear_search_tests(value_to_key, test_values, log_file);
// Run binary search tests and append output to the log file
run_binary_search_tests(sorted_data, test_values, log_file);
// Run hash table search tests and append output to the log file
run_hash_table_search_tests(value_to_key, test_values, log_file);
// Run ternary search tests and append output to the log file
run_ternary_search_tests(sorted_data, test_values, log_file);
// Run exponential search tests and append output to the log file
run_exponential_search_tests(sorted_data, test_values, log_file);

// Close the log file
log_file.close();

return 0;
}

std::pair<std::unordered_map<std::string, std::string>, std::unordered_map<std::string, std::string>> read_data(const std::string& filename) {
    std::unordered_map<std::string, std::string> hexData;
    std::unordered_map<std::string, std::string> value_to_key;

    std::ifstream file(filename);
    std::string line;
    std::cout << "Reading data from " << filename << std::endl;
    int count = 10000;
    progresscpp::ProgressBar bar(count, 70, '#', '-');
    while (std::getline(file, line)) {
        size_t delimiter = line.find('=');
        std::string key = line.substr(0, delimiter);
        std::string value = line.substr(delimiter + 1);
        hexData[key] = value;
        value_to_key[value] = key;
        ++bar;
        bar.display();
    }
    bar.done();
    file.close();

    return {hexData, value_to_key};
}

std::vector<std::string> pick_random_values(const std::unordered_map<std::string, std::string>& data, size_t count) {
    std::vector<std::pair<std::string, std::string>> temp_values;
    std::vector<std::string> values;

    std::sample(data.begin(), data.end(), std::back_inserter(temp_values), count, std::mt19937{std::random_device{}()});
    for (const auto& pair : temp_values) {
        values.push_back(pair.second);
    }

    return values;
}


std::optional<std::string> find_key_by_linear_search(const std::unordered_map<std::string, std::string>& value_to_key, const std::string& value) {
    for (const auto& [key, val] : value_to_key) {
        if (val == value) {
            return key;
        }
    }
    return std::nullopt;
}

std::optional<std::string> find_key_by_binary_search(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::string& value) {
    auto it = std::lower_bound(sorted_data.begin(), sorted_data.end(), value, [](const auto& a, const auto& b) {
        return a.second < b;
    });

    if (it != sorted_data.end() && it->second == value) {
        return it->first;
    } else {
        return std::nullopt;
    }
}

template<typename Func, typename Container>
void run_tests(const std::string& test_name, Func func, const Container& data, const std::vector<std::string>& test_values, std::ofstream& log_file) {
    std::vector<double> times;
    std::optional<std::string> result;
    double total_time = 0.0;
    double best_time = std::numeric_limits<double>::max();
    double worst_time = 0.0;
    std::cout << "Running " << test_name << " tests..." << std::endl;
    progresscpp::ProgressBar bar(test_values.size(), 70, '#', '-');
    for (const auto& value : test_values) {
        auto start_time = std::chrono::high_resolution_clock::now();
        result = func(data, value);
        auto end_time = std::chrono::high_resolution_clock::now();
        double elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() / 1000000.0;
        total_time += elapsed_time;
        times.push_back(elapsed_time);
        if (elapsed_time < best_time) {
            best_time = elapsed_time;
        }
        if (elapsed_time > worst_time) {
            worst_time = elapsed_time;
        }
        ++bar;
        bar.display();
    }
    bar.done();
    double avg_time = total_time / static_cast<double>(test_values.size());

    log_file << test_name << " Test Results: " << std::endl;
    log_file << "  Best Time: " << best_time << " ms" << std::endl;
    log_file << "  Average Time: " << avg_time << " ms" << std::endl;
    log_file << "  Worst Time: " << worst_time << " ms" << std::endl;
    if (result) {
        log_file << "  Found Key: " << *result << std::endl;
    } else {
        log_file << "  Key Not Found" << std::endl;
    }
    log_file << std::endl;
}

void run_linear_search_tests(const std::unordered_map<std::string, std::string>& value_to_key, const std::vector<std::string>& test_values, std::ofstream& log_file) {
    run_tests("Linear Search Test", find_key_by_linear_search, value_to_key, test_values, log_file);
}

void run_binary_search_tests(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::vector<std::string>& test_values, std::ofstream& log_file) {
    run_tests("Binary Search Test", find_key_by_binary_search, sorted_data, test_values, log_file);
}

void run_custom_search_tests(const std::unordered_map<std::string, std::string>& value_to_key, const std::vector<std::string>& test_values, std::ofstream& log_file) {
    run_tests("Custom Search Test", find_key_by_value, value_to_key, test_values, log_file);
}

std::optional<std::string> find_key_by_value(const std::unordered_map<std::string, std::string>& value_to_key, const std::string& value_to_find) {
    auto iter = value_to_key.find(value_to_find);
    if (iter != value_to_key.end()) {
        return iter->second;
    }
    return std::nullopt;
}

std::optional<std::string> find_key_by_hash_table_search(const std::unordered_map<std::string, std::string>& value_to_key, const std::string& value) {
auto iter = value_to_key.find(value);
if (iter != value_to_key.end()) {
return iter->second;
}
return std::nullopt;
}

std::optional<std::string> find_key_by_ternary_search(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::string& value, int left, int right) {
if (left <= right) {
int mid1 = left + (right - left) / 3;
int mid2 = right - (right - left) / 3;
if (sorted_data[mid1].second == value) {
        return sorted_data[mid1].first;
    }
    if (sorted_data[mid2].second == value) {
        return sorted_data[mid2].first;
    }

    if (value < sorted_data[mid1].second) {
        return find_key_by_ternary_search(sorted_data, value, left, mid1 - 1);
} else if (value > sorted_data[mid2].second) {
return find_key_by_ternary_search(sorted_data, value, mid2 + 1, right);
} else {
return find_key_by_ternary_search(sorted_data, value, mid1 + 1, mid2 - 1);
}
}
return std::nullopt;
}

std::optional<std::string> find_key_by_exponential_search(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::string& value) {
if (sorted_data[0].second == value) {
return sorted_data[0].first;
}
int i = 1;
while (i < static_cast<int>(sorted_data.size()) && sorted_data[i].second <= value) {
    i *= 2;
}

int right = std::min(i, static_cast<int>(sorted_data.size() - 1));
// Perform a binary search within the found range
auto it = std::lower_bound(sorted_data.begin() + (i / 2), sorted_data.begin() + right, value, [](const auto& a, const auto& b) {
    return a.second < b;
});

if (it != sorted_data.end() && it->second == value) {
    return it->first;
} else {
    return std::nullopt;
}
}

void run_hash_table_search_tests(const std::unordered_map<std::string, std::string>& value_to_key, const std::vector<std::string>& test_values, std::ofstream& log_file) {
    run_tests("Hash Table Search Test", find_key_by_hash_table_search, value_to_key, test_values, log_file);
}

void run_ternary_search_tests(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::vector<std::string>& test_values, std::ofstream& log_file) {
    run_tests("Ternary Search Test", [&](const auto& data, const auto& value) {
    return find_key_by_ternary_search(data, value, 0, data.size() - 1);
}, sorted_data, test_values, log_file);
}

void run_exponential_search_tests(const std::vector<std::pair<std::string, std::string>>& sorted_data, const std::vector<std::string>& test_values, std::ofstream& log_file) {
    run_tests("Exponential Search Test", find_key_by_binary_search, sorted_data, test_values, log_file);
}
