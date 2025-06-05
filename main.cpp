#include "CSVReader.hpp"
#include "Teams.hpp"
#include "TeamScheduler.hpp"
#include <iostream>
#include <fstream>
#include <random>

static std::mt19937_64 rng{ std::random_device{}() };

void displayWelcomeMenu() {
    std::cout << "========================================================\n";
    std::cout << "            Welcome to the Annual Spring Major         \n";
    std::cout << "========================================================\n";
    std::cout << "Tournament: Asia Pacific University Esports           \n";
    std::cout << "            Championship (APUEC) 2025                  \n";
    std::cout << "Dates     : 3 – 22 May 2025                           \n";
    std::cout << "Location  : Asia Pacific University of Technology &   \n";
    std::cout << "            Innovation (APU), Bukit Jalil, Kuala Lumpur\n";
    std::cout << "--------------------------------------------------------\n";
    std::cout << "Stage Dates:                                          \n";
    std::cout << "  Qualifiers    : 3 -  6 May 2025                      \n";
    std::cout << "  Round of 16   : 7 -  8 May 2025                      \n";
    std::cout << "  Quarter Finals: 10 - 11 May 2025                     \n";
    std::cout << "  Semi Finals   : 14 - 15 May 2025                     \n";
    std::cout << "  Finals        : 22 May 2025                          \n";
    std::cout << "========================================================\n\n";
}

void logTeamResult(const Tournament::Match& m, const Team& w) {
    std::cout << "  Match " << m.matchId
              << ": Winner = " << w.name << "\n";
}

int main() {
    displayWelcomeMenu();

    // 1) Load existing players.csv
    Player players[MAX_PLAYERS];
    std::size_t playerCount = 0;
    if (!readPlayersFromCSV("players.csv", players, playerCount)) {
        return 1;
    }
    std::cout << "Data extracted successfully ("
              << playerCount << " records)\n";

    // 2) Randomize each player's ranking to [1900,2400]
    std::uniform_int_distribution<int> dist(1900, 2400);
    for (std::size_t i = 0; i < playerCount; ++i) {
        players[i].ranking = dist(rng);
    }

    // 3) Rewrite players.csv with the new randomized rankings
    {
        std::ofstream out("players.csv", std::ios::trunc);
        if (!out) {
            std::cerr << "Error: cannot open players.csv for writing\n";
            return 1;
        }
        out << "id,ranking,name,institution,ign\n";
        for (std::size_t i = 0; i < playerCount; ++i) {
            const auto &p = players[i];
            out << p.id << ','
                << p.ranking << ','
                << p.name << ','
                << p.institution << ','
                << p.ign << '\n';
        }
    }

    // 4) Hard-code team names
    constexpr std::size_t TEAM_COUNT = 20;
    const char* teamNames[TEAM_COUNT] = {
      "Astralis","Team Liquid","FaZe Clan","G2 Esports","Natus Vincere",
      "Fnatic","Mousesports","Virtus.pro","Team Vitality",
      "Complexity Gaming","Sentinels","100 Thieves","TSM","OpTic Gaming",
      "Cloud9","Paper Rex","Vision Strikers","Team Envy","FURIA Esports","DRX"
    };

    // 5) Build teams
    Team teams[TEAM_COUNT];
    for (std::size_t t = 0; t < TEAM_COUNT; ++t) {
        teams[t].name = teamNames[t];
        teams[t].memberCount = 0;
    }
    for (std::size_t i = 0; i < playerCount; ++i) {
        std::size_t t = i / TEAM_SIZE;
        teams[t].members[ teams[t].memberCount++ ] = &players[i];
    }

    // 6) Compute each team's average rating
    for (std::size_t t = 0; t < TEAM_COUNT; ++t) {
        int sum = 0;
        for (std::size_t j = 0; j < teams[t].memberCount; ++j) {
            sum += teams[t].members[j]->ranking;
        }
        teams[t].rating = sum / static_cast<int>(teams[t].memberCount);
    }

    // 7) Run the tournament
    Tournament::seedTeams(teams, TEAM_COUNT);
    Tournament::onTeamResult(&logTeamResult);
    Tournament::runTeamStages();

    // 8) Print champion roster
    std::size_t champCount;
    Team** survivors = Tournament::getNextStageTeams(champCount);
    if (champCount == 1) {
        Team* champ = survivors[0];
        std::cout << "\n  🏆 Champion: " << champ->name << "\n\n"
                  << "  Members:\n";
        for (std::size_t i = 0; i < champ->memberCount; ++i) {
            auto* p = champ->members[i];
            std::cout << "    - " << p->name
                      << " (ID: " << p->id
                      << ", Rank: " << p->ranking << ")\n";
        }
        std::cout << "\n└─────────────────── Champion ───────────────────┘\n";
    }

    return 0;
}
