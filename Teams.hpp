#ifndef TEAMS_HPP
#define TEAMS_HPP

#include "CSVReader.hpp" 

constexpr std::size_t TEAM_SIZE = 10;

struct Team {
    std::string  name;
    Player*      members[TEAM_SIZE];
    std::size_t  memberCount = 0;
    int          rating;     // average of member rankings
};

#endif // TEAMS_HPP
