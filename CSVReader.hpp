#ifndef CSV_READER_HPP
#define CSV_READER_HPP

#include <cstddef>
#include <string>

constexpr std::size_t MAX_PLAYERS = 1000;

struct Player {
    std::string id;
    int         ranking;
    std::string name;
    std::string institution;
    std::string ign;
    std::string team;
};

std::string trim(const std::string& s);

// Reads up to MAX_PLAYERS entries from filename into players[].
// On return, count == number of valid rows read.
bool readPlayersFromCSV(const std::string& filename,
                        Player players[],
                        std::size_t& count);

#endif // CSV_READER_HPP
