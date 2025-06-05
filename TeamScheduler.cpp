// TeamScheduler.cpp
#include "TeamScheduler.hpp"
#include "MatchQueue.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <iomanip>
#include <random>

namespace Tournament {

static Team*        g_teams      = nullptr;
static std::size_t  g_teamCount  = 0;
static TeamResultCallback g_cb    = nullptr;

static Team*        g_buffer[MAX_TEAMS];
static std::size_t  g_bufferSize = 0;
static Team*        g_nextBuffer[MAX_TEAMS];
static std::size_t  g_nextCount  = 0;

static std::mt19937_64 rng{ std::random_device{}() };

void seedTeams(Team teams[], std::size_t count) {
    g_teams      = teams;
    g_teamCount  = count;
    g_bufferSize = count;
    for (std::size_t i = 0; i < count; ++i)
        g_buffer[i] = &teams[i];
}

void onTeamResult(TeamResultCallback cb) {
    g_cb = cb;
}

void runTeamStages() {
    std::size_t matchId = 1;
    int roundNum = 1;

    while (g_bufferSize > 1) {
        std::size_t originalCount = g_bufferSize;
        const char* stageName;
        std::size_t target;
        switch (roundNum) {
            case 1: stageName = "Qualifiers";     target = 16; break;
            case 2: stageName = "Round of 16";    target = 8;  break;
            case 3: stageName = "Quarter Finals"; target = 4;  break;
            case 4: stageName = "Semi Finals";    target = 2;  break;
            case 5: stageName = "Finals";         target = 1;  break;
            default: stageName = "Round";         target = 1;  break;
        }

        std::cout << "\nPress ENTER to start " << stageName << "...";
        std::cin.get();

        std::cout << "\n┌───────────────────────────────────── " 
          << stageName 
          << " ─────────────────────────────────────┐\n\n";


        std::sort(g_buffer, g_buffer + g_bufferSize,
                  [](Team* a, Team* b){ return a->rating > b->rating; });

        std::size_t elimMatches = (g_bufferSize > target)
                                  ? (g_bufferSize - target)
                                  : 0;
        std::size_t twoE     = elimMatches * 2;
        std::size_t byeCount = g_bufferSize - twoE;
        g_nextCount = 0;

        MatchQueue queue;

        for (std::size_t i = 0; i < byeCount; ++i)
            g_nextBuffer[g_nextCount++] = g_buffer[i];

        std::vector<std::pair<Team*,Team*>> pairings;
        for (std::size_t i = 0; i < elimMatches; ++i)
            pairings.emplace_back(g_buffer[byeCount + i], g_buffer[g_bufferSize - 1 - i]);

        const int dashLen = 15;
        const int gapLen  = 20;
        
        for (auto &pair : pairings) {
            Team *A = pair.first, *B = pair.second;
            std::cout << std::left << std::setw(16) << A->name
                      << std::string(dashLen, '-') << "|"
                      << std::string(gapLen, ' ') << "|"
                      << std::string(dashLen, '-')
                      << B->name << "\n\n";

            Team* winner;
            if (roundNum >= 4) {
                winner = std::bernoulli_distribution(0.5)(rng) ? A : B;
            } else {
                winner = (A->rating >= B->rating) ? A : B;
            }

            std::cout << std::setw(35) << " " << "Winner: " << winner->name << "\n\n";

            queue.enqueue({matchId++, A, B});
            g_nextBuffer[g_nextCount++] = winner;
        }

        g_bufferSize = g_nextCount;
        for (std::size_t i = 0; i < g_nextCount; ++i)
            g_buffer[i] = g_nextBuffer[i];

         std::cout << "\n└───────────────────────────────────── " 
          << stageName 
          << " ─────────────────────────────────────┘\n\n";



        ++roundNum;
    }

    if (g_bufferSize == 1) {
        std::cout << "┌─────────────────── Champion ───────────────────┐\n";
    }
}

Team** getNextStageTeams(std::size_t& outCount) {
    outCount = g_bufferSize;
    return g_buffer;
}

} // namespace Tournament
