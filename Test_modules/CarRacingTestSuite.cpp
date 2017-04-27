#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Race.hpp"
#include "CarMock.hpp"
#include "TrackMock.hpp"
#include <vector>
#include "TeamMock.hpp"
#include <functional>

using ::testing::Return;
using ::testing::ReturnRef;
using namespace std;

class CarRacingTestSuite : public ::testing::Test
{
public:
    CarMock m_carMock{};
    Race m_race;
    TrackMock m_trackMock{};

};

TEST_F(CarRacingTestSuite, notFullyPreparedCarShouldNotJoinTheRace)
{
    EXPECT_CALL(m_carMock, statusOfTire()).WillOnce(Return(100));
    EXPECT_CALL(m_carMock, statusOfEngine()).WillOnce(Return(100));
    EXPECT_CALL(m_carMock, statusOfSuspension()).WillOnce(Return(20));

    ASSERT_FALSE(m_race.validate(&m_carMock));
}

TEST_F(CarRacingTestSuite, fullyPreparedCarShouldBeAdmitted)
{
    EXPECT_CALL(m_carMock, statusOfTire()).WillOnce(Return(100));
    EXPECT_CALL(m_carMock, statusOfEngine()).WillOnce(Return(100));
    EXPECT_CALL(m_carMock, statusOfSuspension()).WillOnce(Return(100));

    ASSERT_TRUE(m_race.validate(&m_carMock));
}

struct TrackParams
{
    int length;
    int turns;
};

struct RaceCalcTimeTestParams
{
    EngineQuality engineQuality;
    Handling      handling;
    TrackParams   trackParams;
    float         expectedTime;
};

class RaceCalcTimeTestSuite
    : public CarRacingTestSuite,
      public ::testing::WithParamInterface<RaceCalcTimeTestParams>
      {};

INSTANTIATE_TEST_CASE_P(test_RaceCalcTimeWithDifferentParams,
                        RaceCalcTimeTestSuite,
                        ::testing::Values(
                            RaceCalcTimeTestParams{EngineQuality::High, Handling::Good, {500, 6}, 28.0},
                            RaceCalcTimeTestParams{EngineQuality::Low, Handling::Good, {500, 6}, 33.0},
                            RaceCalcTimeTestParams{EngineQuality::High, Handling::Bad, {500, 6}, 34.0},
                            RaceCalcTimeTestParams{EngineQuality::Low, Handling::Bad, {500, 6}, 39.0},
                            RaceCalcTimeTestParams{EngineQuality::High, Handling::Good, {1000, 3}, 51.5},
                            RaceCalcTimeTestParams{EngineQuality::Low, Handling::Good, {1000, 3}, 61.5},
                            RaceCalcTimeTestParams{EngineQuality::High, Handling::Bad, {1000, 3}, 54.5},
                            RaceCalcTimeTestParams{EngineQuality::Low, Handling::Bad, {1000, 3}, 64.5}));

TEST_P(RaceCalcTimeTestSuite, RaceCalcTimeInDifferentParams)
{
    EXPECT_CALL(m_carMock, qualityOfEngine()).WillOnce(Return(GetParam().engineQuality));
    EXPECT_CALL(m_carMock, handling()).WillOnce(Return(GetParam().handling));
    EXPECT_CALL(m_trackMock, getLength()).WillOnce(Return(GetParam().trackParams.length));
    EXPECT_CALL(m_trackMock, getTurns()).WillOnce(Return(GetParam().trackParams.turns));

    ASSERT_EQ(GetParam().expectedTime, m_race.calcTime(&m_carMock, m_trackMock));
}

TEST_F(CarRacingTestSuite, TeamWithLessTimeShouldWin)
{
    CarMock l_car1;
    CarMock l_car2;
    TeamMock l_team1;
    TeamMock l_team2;
    vector<ITeam*> l_teams{&l_team1, &l_team2};
    vector<int> l_seq{2, 1};

    EXPECT_CALL(l_team1, getCar()).WillRepeatedly(Return(&l_car1));
    EXPECT_CALL(l_team2, getCar()).WillRepeatedly(Return(&l_car2));
    EXPECT_CALL(l_team1, getId()).WillRepeatedly(Return(1));
    EXPECT_CALL(l_team2, getId()).WillRepeatedly(Return(2));
    EXPECT_CALL(l_team1, getQualificationTime()).WillRepeatedly(Return(35.0));
    EXPECT_CALL(l_team2, getQualificationTime()).WillRepeatedly(Return(32.0));

    EXPECT_CALL(l_car1, statusOfTire()).WillOnce(Return(100));
    EXPECT_CALL(l_car1, statusOfEngine()).WillOnce(Return(100));
    EXPECT_CALL(l_car1, statusOfSuspension()).WillOnce(Return(100));
    EXPECT_CALL(l_car1, qualityOfEngine()).WillOnce(Return(EngineQuality::High));
    EXPECT_CALL(l_car1, handling()).WillOnce(Return(Handling::Good));

    EXPECT_CALL(l_car2, statusOfTire()).WillOnce(Return(100));
    EXPECT_CALL(l_car2, statusOfEngine()).WillOnce(Return(100));
    EXPECT_CALL(l_car2, statusOfSuspension()).WillOnce(Return(100));
    EXPECT_CALL(l_car2, qualityOfEngine()).WillOnce(Return(EngineQuality::High));
    EXPECT_CALL(l_car2, handling()).WillOnce(Return(Handling::Good));

    EXPECT_CALL(m_trackMock, getLength()).WillRepeatedly(Return(500));
    EXPECT_CALL(m_trackMock, getTurns()).WillRepeatedly(Return(6));

    ASSERT_EQ(l_seq, m_race.run(l_teams, m_trackMock));

}

TEST_F(CarRacingTestSuite, MoreThanSixValidedTeamsAreNotAllowed)
{
    CarMock l_car1;
    TeamMock l_team1, l_team2, l_team3, l_team4, l_team5, l_team6, l_team7;

    vector<ITeam*> l_teams{&l_team1, &l_team2, &l_team3, &l_team4, &l_team5, &l_team6, &l_team7};

    EXPECT_CALL(l_team1, getCar()).WillRepeatedly(Return(&l_car1));
    EXPECT_CALL(l_team2, getCar()).WillRepeatedly(Return(&l_car1));
    EXPECT_CALL(l_team3, getCar()).WillRepeatedly(Return(&l_car1));
    EXPECT_CALL(l_team4, getCar()).WillRepeatedly(Return(&l_car1));
    EXPECT_CALL(l_team5, getCar()).WillRepeatedly(Return(&l_car1));
    EXPECT_CALL(l_team6, getCar()).WillRepeatedly(Return(&l_car1));
    EXPECT_CALL(l_team7, getCar()).WillRepeatedly(Return(&l_car1));


    EXPECT_CALL(l_car1, statusOfTire()).WillRepeatedly(Return(100));
    EXPECT_CALL(l_car1, statusOfEngine()).WillRepeatedly(Return(100));
    EXPECT_CALL(l_car1, statusOfSuspension()).WillRepeatedly(Return(100));

    ASSERT_THROW(m_race.run(l_teams, m_trackMock), out_of_range);

}

TEST_F(CarRacingTestSuite, LessThanTwoValidedTeamsAreNotAllowed)
{
    CarMock l_car1;
    TeamMock l_team1;

    vector<ITeam*> l_teams{&l_team1};

    EXPECT_CALL(l_team1, getCar()).WillRepeatedly(Return(&l_car1));

    EXPECT_CALL(l_car1, statusOfTire()).WillRepeatedly(Return(100));
    EXPECT_CALL(l_car1, statusOfEngine()).WillRepeatedly(Return(100));
    EXPECT_CALL(l_car1, statusOfSuspension()).WillRepeatedly(Return(100));

    ASSERT_THROW(m_race.run(l_teams, m_trackMock), out_of_range);

}


