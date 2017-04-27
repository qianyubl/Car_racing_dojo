#pragma once

#include <vector>
#include <functional>
#include "ITeam.hpp"

class ICar;
class ITrack;

using namespace std;

class Race
{
public:
    vector<int> run(const vector<ITeam*>&, const ITrack&);
    bool checkIfCarStausIsvalid(const ICar*);
    float calcTime(const ICar*, const ITrack&);
    bool checkIfNumberOfTeamsIsValid(vector<ITeam*>&);
    void selectTeamsAbleToJoinRace(const vector<ITeam*>&, vector<ITeam*>&);
    void sortTeamsOrdersBaseOnQualificationTime(vector<ITeam*>&);
    void getTimeOfEachTeamAfterFirstRound(vector<ITeam*>& p_Teams, vector<pair<ITeam*, float>>&, const ITrack&);
    void sortTeamsWithTimeOrdersBaseOnTime(vector<pair<ITeam*, float>>&);
    void headToHeadAfterFirstRoundTeamsWithBetterEngineWillGo1SecondFaster(vector<pair<ITeam*, float>>&);
    void getTotalTimeOfEachTeamAfterRaceFinished(vector<pair<ITeam*, float>>& p_TeamsWithTime, const ITrack& p_track);
    void getResultsSeqOfTeams(vector<pair<ITeam*, float>>&, vector<int>&);
};
