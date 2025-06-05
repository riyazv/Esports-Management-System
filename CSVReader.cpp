#include "CSVReader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// Trim whitespace from both ends of a string
std::string trim(const std::string& s) {
    auto a = s.find_first_not_of(" \t\r\n");
    auto b = s.find_last_not_of (" \t\r\n");
    return (a == std::string::npos)
         ? ""
         : s.substr(a, b - a + 1);
}

bool readPlayersFromCSV(const std::string& filename,
                        Player players[],
                        std::size_t& count) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return false;
    }

    std::string line;
    // Skip header
    if (!std::getline(file, line)) {
        std::cerr << "Empty file: " << filename << "\n";
        return false;
    }

    count = 0;
    while (count < MAX_PLAYERS && std::getline(file, line)) {
        std::stringstream ss(line);
        std::string f[5];
        bool ok = true;
        for (int i = 0; i < 5; ++i) {
            if (!std::getline(ss, f[i], ',')) { ok = false; break; }
            f[i] = trim(f[i]);
        }
        if (!ok) continue;

        Player& p = players[count];
        p.id = f[0];
        try {
            p.ranking = std::stoi(f[1]);
        } catch (...) {
            std::cerr << "Skipping line " << (count + 2)
                      << " (bad ranking)\n";
            continue;
        }
        p.name        = f[2];
        p.institution = f[3];
        p.ign         = f[4];
        ++count;
    }
    return true;
}
