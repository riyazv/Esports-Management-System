#ifndef TEAM_SCHEDULER_HPP
#define TEAM_SCHEDULER_HPP

#include <cstddef>
#include "Teams.hpp"

namespace Tournament {
    constexpr std::size_t MAX_TEAMS = MAX_PLAYERS / TEAM_SIZE;

    struct Match {
        std::size_t matchId;
        Team*       t1;
        Team*       t2;
    };

    void seedTeams(Team teams[], std::size_t count);
    using TeamResultCallback = void (*)(const Match&, const Team& winner);
    void onTeamResult(TeamResultCallback cb);
    void runTeamStages();
    Team** getNextStageTeams(std::size_t& outCount);
}

#endif // TEAM_SCHEDULER_HPP
