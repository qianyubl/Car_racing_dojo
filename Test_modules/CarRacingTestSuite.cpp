#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Race.hpp"
#include "TeamMock.hpp"
#include "ITeam.hpp"
#include "Track.hpp"
#include <algorithm>

using ::testing::Return;

TEST(CarRacingTestSuite, raceTeam2WinTeam1)
{
    Race l_race;
    Track l_track;
    vector<ITeam*> l_teams;
    vector<int> l_expectTeamsSeq{2, 1};

    TeamMock l_t1, l_t2;
    l_teams.push_back(&l_t1);
    l_teams.push_back(&l_t2);

    EXPECT_CALL(l_t1, getTotalTime()).WillOnce(Return(100));
    EXPECT_CALL(l_t2, getTotalTime()).WillOnce(Return(10));

    EXPECT_CALL(l_t1, getId()).WillOnce(Return(1));
    EXPECT_CALL(l_t2, getId()).WillOnce(Return(2));

    ASSERT_EQ(l_expectTeamsSeq, l_race.run(l_teams, l_track));

}

TEST(CarRacingTestSuite, team1WinsTeam2)
{
    Race l_race;
    Track l_track;
    vector<ITeam*> l_teams;
    vector<int> l_expectTeamsSeq{1, 2};

    TeamMock l_t1, l_t2;
    l_teams.push_back(&l_t1);
    l_teams.push_back(&l_t2);

    EXPECT_CALL(l_t1, getTotalTime()).WillOnce(Return(10));
    EXPECT_CALL(l_t2, getTotalTime()).WillOnce(Return(100));

    EXPECT_CALL(l_t1, getId()).WillOnce(Return(1));
    EXPECT_CALL(l_t2, getId()).WillOnce(Return(2));

    ASSERT_EQ(l_expectTeamsSeq, l_race.run(l_teams, l_track));

}


