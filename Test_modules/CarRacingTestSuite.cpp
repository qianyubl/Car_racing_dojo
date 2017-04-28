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
    void setCarAllAttributes(CarMock& p_car, int p_statusOfTire, int p_statusOfEngine, int p_statusOfSuspension, EngineQuality p_qualityOfEngine, Handling p_handling)
    {
        setCarStatus(p_car, p_statusOfTire, p_statusOfEngine, p_statusOfSuspension);
        EXPECT_CALL(p_car, qualityOfEngine()).WillRepeatedly(Return(p_qualityOfEngine));
        EXPECT_CALL(p_car, handling()).WillRepeatedly(Return(p_handling));
    }


    void setCarStatus(CarMock& p_car, int p_statusOfTire, int p_statusOfEngine, int p_statusOfSuspension)
    {
        EXPECT_CALL(p_car, statusOfTire()).WillRepeatedly(Return(p_statusOfTire));
        EXPECT_CALL(p_car, statusOfEngine()).WillRepeatedly(Return(p_statusOfEngine));
        EXPECT_CALL(p_car, statusOfSuspension()).WillRepeatedly(Return(p_statusOfSuspension));
    }

    void setTeamAttributes(TeamMock& p_team, const CarMock* p_car, int p_id, float p_qualificationTime)
    {
        EXPECT_CALL(p_team, getCar()).WillRepeatedly(Return(p_car));
        EXPECT_CALL(p_team, getId()).WillRepeatedly(Return(p_id));
        EXPECT_CALL(p_team, getQualificationTime()).WillRepeatedly(Return(p_qualificationTime));
    }
    CarMock m_carMock{};
    Race m_race;
    TrackMock m_trackMock{};

};

TEST_F(CarRacingTestSuite, notFullyPreparedCarShouldNotJoinTheRace)
{
    setCarStatus(m_carMock, 100, 100, 20);

    ASSERT_FALSE(m_race.checkIfCarStausIsvalid(&m_carMock));
}

TEST_F(CarRacingTestSuite, fullyPreparedCarShouldBeAdmitted)
{
    setCarStatus(m_carMock, 100, 100, 100);

    ASSERT_TRUE(m_race.checkIfCarStausIsvalid(&m_carMock));
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

    setTeamAttributes(l_team1, &l_car1, 1, 35.0);
    setTeamAttributes(l_team2, &l_car2, 2, 32.0);

    setCarAllAttributes(l_car1, 100, 100, 100, EngineQuality::High,Handling::Good);
    setCarAllAttributes(l_car2, 100, 100, 100, EngineQuality::High,Handling::Good);

    EXPECT_CALL(m_trackMock, getLength()).WillRepeatedly(Return(500));
    EXPECT_CALL(m_trackMock, getTurns()).WillRepeatedly(Return(6));

    ASSERT_EQ(l_seq, m_race.run(l_teams, m_trackMock));

}

TEST_F(CarRacingTestSuite, TeamWithEqualTimeAfterOneLapWithBetterEngineShouldWin)
{
    CarMock l_car1, l_car2, l_car3;
    TeamMock l_team1, l_team2, l_team3;
    vector<ITeam*> l_teams{&l_team1, &l_team2, &l_team3};

    setTeamAttributes(l_team1, &l_car1, 1, 35.0);
    setTeamAttributes(l_team2, &l_car2, 2, 32.0);
    setTeamAttributes(l_team3, &l_car3, 3, 38.0);

    setCarAllAttributes(l_car1, 100, 100, 100, EngineQuality::Low,Handling::Good);
    setCarAllAttributes(l_car2, 100, 100, 100, EngineQuality::High,Handling::Good);
    setCarAllAttributes(l_car3, 100, 100, 100, EngineQuality::High,Handling::Bad);

    EXPECT_CALL(m_trackMock, getLength()).WillRepeatedly(Return(500));
    EXPECT_CALL(m_trackMock, getTurns()).WillRepeatedly(Return(6));

    vector<int> l_seq{2, 1, 3};
    ASSERT_EQ(l_seq, m_race.run(l_teams, m_trackMock));

}

TEST_F(CarRacingTestSuite, MoreThanSixValidedTeamsAreNotAllowed)
{
    CarMock l_car1;
    TeamMock l_team1, l_team2, l_team3, l_team4, l_team5, l_team6, l_team7;

    vector<ITeam*> l_teams{&l_team1, &l_team2, &l_team3, &l_team4, &l_team5, &l_team6, &l_team7};

    for(auto p : l_teams)
    {
        EXPECT_CALL(*(dynamic_cast<TeamMock*>(p)), getCar()).WillRepeatedly(Return(&l_car1));
    }

    setCarStatus(l_car1, 100, 100, 100);

    ASSERT_THROW(m_race.run(l_teams, m_trackMock), out_of_range);

}

TEST_F(CarRacingTestSuite, LessThanTwoValidedTeamsAreNotAllowed)
{
    CarMock l_car1;
    TeamMock l_team1;

    vector<ITeam*> l_teams{&l_team1};

    EXPECT_CALL(l_team1, getCar()).WillRepeatedly(Return(&l_car1));

    setCarStatus(l_car1, 100, 100, 100);

    ASSERT_THROW(m_race.run(l_teams, m_trackMock), out_of_range);

}


