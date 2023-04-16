#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <optional>

std::pair<std::unordered_map<std::string, std::string>, std::unordered_map<std::string, std::string>> read_data(const std::string& filename);
std::optional<std::string> find_key_by_value(const std::unordered_map<std::string, std::string>& value_to_key, const std::string& value_to_find);

int main() {
    std::string filename = "signatures.db";
    auto [hexData, value_to_key] = read_data(filename);

    // Search for a specific hex value and find its corresponding key
    std::string hex_value_to_find = "255044462d312e340a332030206f626a203c3c0a2f4c656e677468203139";
    std::optional<std::string> key_found = find_key_by_value(value_to_key, hex_value_to_find);

    if (key_found.has_value()) {
        std::cout << "Key for the hex value " << hex_value_to_find << " is: " << key_found.value() << std::endl;
    } else {
        std::cout << "Hex value " << hex_value_to_find << " not found." << std::endl;
    }

    return 0;
}

std::pair<std::unordered_map<std::string, std::string>, std::unordered_map<std::string, std::string>> read_data(const std::string& filename) {
    std::unordered_map<std::string, std::string> hexData;
    std::unordered_map<std::string, std::string> value_to_key;
    std::ifstream infile(filename);
    std::string line;

    if (infile.is_open()) {
        while (std::getline(infile, line)) {
            size_t delimiter_position = line.find('=');
            if (delimiter_position != std::string::npos) {
                std::string key = line.substr(0, delimiter_position);
                std::string value = line.substr(delimiter_position + 1);
                hexData[key] = value;
                value_to_key[value] = key;
            }
        }
        infile.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return {hexData, value_to_key};
}

std::optional<std::string> find_key_by_value(const std::unordered_map<std::string, std::string>& value_to_key, const std::string& value_to_find) {
    auto iter = value_to_key.find(value_to_find);
    if (iter != value_to_key.end()) {
        return iter->second;
    }
    return std::nullopt;
}
