#include "constants.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>

// Define constants
int CENTER_GRAV = 0;
float GLOBAL_SIZE = 0.02f;
glm::vec3 G = glm::vec3(0.f, -1.f, 0.f);
float g = 10.f;
float EPS = 1e-7f;
float rebounce_factor = 0.9f;

std::unordered_map<std::string, std::string> parseINI(const std::string& filePath) {
    std::unordered_map<std::string, std::string> values;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open " + filePath);
    }

    std::string line, section;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        line.erase(0, line.find_first_not_of(" \t")); // Trim leading spaces
        if (line.empty() || line[0] == ';' || line[0] == '#') continue; // Skip comments
        if (line[0] == '[') {
            section = line.substr(1, line.find(']') - 1);
        } else {
            auto delimiterPos = line.find('=');
            auto key = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            values[section + "." + key] = value;
        }
    }
    return values;
}

void loadConstants(const std::string& iniFilePath) {
    std::unordered_map<std::string, std::string> reader = parseINI(iniFilePath);

    std::cout << reader["Physics.CENTER_GRAV"] << std::endl;

    CENTER_GRAV = std::stoi(reader["Physics.CENTER_GRAV"]);
    GLOBAL_SIZE = std::stof(reader["Physics.GLOBAL_SIZE"]);
    G = glm::vec3(
        std::stof(reader["Physics.Gx"]),
        std::stof(reader["Physics.Gy"]),
        std::stof(reader["Physics.Gz"])
    );
    g = std::stof(reader["Physics.g"]);
    EPS = std::stof(reader["Physics.EPS"]);
    rebounce_factor = std::stof(reader["Physics.rebounce_factor"]);
}