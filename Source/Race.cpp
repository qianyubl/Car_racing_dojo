#include "Race.hpp"
#include "ICar.hpp"
#include "ITrack.hpp"
#include "algorithm"
#include "iostream"
#include <functional>

vector<int> Race::run(const vector<ITeam*>& p_teams, const ITrack& p_track)
{
    vector<pair<ITeam*, float>> l_vecSeq;
    vector<ITeam*> l_teams;

    selectTeamsAbleToJoinRace(p_teams, l_teams);

    if(not checkIfNumberOfTeamsIsValid(l_teams))
    {
        throw out_of_range(" valid teams size is out of range ");
    }

    sortTeamsOrdersBaseOnQualificationTime(l_teams);

    getTimeOfEachTeamAfterFirstRound(l_teams, l_vecSeq, p_track);

    sortTeamsWithTimeOrdersBaseOnTime(l_vecSeq);
//    for(auto p : l_vecSeq)
//      cout << p.first->getId() << " " << p.second << endl;

    headToHeadAfterFirstRoundTeamsWithBetterEngineWillGo1SecondFaster(l_vecSeq);
//    for(auto p : l_vecSeq)
//      cout << p.first->getId() << " " << p.second << endl;

    getTotalTimeOfEachTeamAfterRaceFinished(l_vecSeq, p_track);
//    for(auto p:l_vecSeq)
//        cout << p.first->getId() << " " << p.second << endl;
    forHeadToHeadSituation(l_vecSeq);
    sortTeamsWithTimeOrdersBaseOnTime(l_vecSeq);
//    for(auto p:l_vecSeq)
//        cout << p.first->getId() << " " << p.second << endl;

    vector<int> l_res;
    getResultsSeqOfTeams(l_vecSeq, l_res);

    return move(l_res);
}

bool Race::checkIfCarStausIsvalid(const ICar* p_car)
{
    return (100 == p_car -> statusOfTire() and
            100 == p_car -> statusOfEngine() and
            100 == p_car -> statusOfSuspension());
}

bool Race::checkIfNumberOfTeamsIsValid(vector<ITeam*>& p_teams)
{
    return p_teams.size() < 7 and p_teams.size() > 1;
}

void Race::selectTeamsAbleToJoinRace(const vector<ITeam*>& p_orginalTeams, vector<ITeam*>& p_selectedTeams)
{
    for (auto p : p_orginalTeams)
    {
        if(checkIfCarStausIsvalid(p->getCar()))
        {
            p_selectedTeams.push_back(p);
        }
    }
}

void Race::sortTeamsOrdersBaseOnQualificationTime(vector<ITeam*>& p_Teams)
{

    sort(p_Teams.begin(), p_Teams.end(),
            [](auto p1, auto p2){ return p1->getQualificationTime() < p2->getQualificationTime();});


}

void Race::getTimeOfEachTeamAfterFirstRound(vector<ITeam*>& p_Teams, vector<pair<ITeam*, float>>& p_TeamsWithTime, const ITrack& p_track)
{
    float l_startOrderTimeToBeAdded = 0.0;
    for_each(p_Teams.begin(), p_Teams.end(),
             [&](auto p){ p_TeamsWithTime.push_back(make_pair(p, (l_startOrderTimeToBeAdded++) + this -> calcTime(p->getCar(),p_track)));});

}

void Race::sortTeamsWithTimeOrdersBaseOnTime(vector<pair<ITeam*, float>>& p_TeamsWithTime)
{
    sort(p_TeamsWithTime.begin(), p_TeamsWithTime.end(),
             [](auto p1, auto p2){ return p1.second < p2.second;});
}
//struct
//{
//    bool operator()(pair<ITeam*, float>& p1, pair<ITeam*, float>& p2)
//    {
//        return p1.second < p2.second;
//    }
//}customLess;

//void Race::sortTeamsWithTimeOrdersBaseOnTime(vector<pair<ITeam*, float>>& p_TeamsWithTime)
//{
//    sort(p_TeamsWithTime.begin(), p_TeamsWithTime.end(), customLess);
//}

void Race::headToHeadAfterFirstRoundTeamsWithBetterEngineWillGo1SecondFaster(vector<pair<ITeam*, float>>& p_TeamsWithTime)
{
    auto l_resultIter = p_TeamsWithTime.begin();
    for(auto l_iter = p_TeamsWithTime.begin(); l_iter != p_TeamsWithTime.end();)
    {
        l_resultIter = find_if(l_iter, p_TeamsWithTime.end(),[&](auto p){return p.second != l_iter->second;});
        if(any_of(l_iter, l_resultIter, [](auto p){return EngineQuality::Low == p.first->getCar()->qualityOfEngine();}))
        {
            for_each(l_iter, l_resultIter, [](auto& p)
                                           {
                                               if(EngineQuality::High == p.first->getCar()->qualityOfEngine())
                                               {
                                                   p.second -= 1.0;
                                               }
                                           });

        }
        l_iter = l_resultIter;
    }

}

void Race::forHeadToHeadSituation(vector<pair<ITeam*, float>>& p_TeamsWithTime)
{
    auto l_resultIter = p_TeamsWithTime.begin();
    for(auto l_iter = p_TeamsWithTime.begin(); l_iter != p_TeamsWithTime.end();)
    {
        l_resultIter = find_if(l_iter, p_TeamsWithTime.end(),[&](auto p){return p.second != l_iter->second;});
        if(any_of(l_iter, l_resultIter, [](auto p){return Handling::Bad == p.first->getCar()->handling();}))
        {
            for_each(l_iter, l_resultIter, [](auto& p)
                                           {
                                               if(Handling::Good == p.first->getCar()->handling())
                                               {
                                                   p.second -= 1.0;
                                               }
                                           });

        }
        l_iter = l_resultIter;
    }

}

void Race::getTotalTimeOfEachTeamAfterRaceFinished(vector<pair<ITeam*, float>>& p_TeamsWithTime, const ITrack& p_track)
{
    for_each(p_TeamsWithTime.begin(), p_TeamsWithTime.end(),
                     [&](auto& p){ p.second += this -> calcTime(p.first->getCar(),p_track);});
}

void Race::getResultsSeqOfTeams(vector<pair<ITeam*, float>>& p_TeamsWithTime, vector<int>& p_results)
{
    for_each(p_TeamsWithTime.begin(), p_TeamsWithTime.end(),
                 [&](auto p){ p_results.push_back(p.first->getId());});
}

float Race::calcTime(const ICar* p_car, const ITrack& p_track)
{
    int l_timeOfLength = (EngineQuality::High == p_car -> qualityOfEngine()) ? 5 : 6;
    float l_timeOfTurns = (Handling::Good == p_car -> handling()) ? 0.5 : 1.5;

    return p_track.getLength() / 100 * l_timeOfLength + p_track.getTurns()* l_timeOfTurns;
}

