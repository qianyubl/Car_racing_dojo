#include "Race.hpp"
#include "ICar.hpp"
#include "ITrack.hpp"
#include "algorithm"

vector<int> Race::run(const vector<ITeam*>& p_teams, const ITrack& p_track)
{
    vector<pair<ITeam*, float>> l_vecSeq;
    vector<ITeam*> l_teams;

    for (auto p : p_teams)
    {
        if(validate(p->getCar()))
        {
            l_teams.push_back(p);
        }
    }

    if(l_teams.size() > 6 or l_teams.size() < 2)
    {
        throw out_of_range(" validate teams size is out of range ");
    }

    sort(l_teams.begin(), l_teams.end(),
         [](auto p1, auto p2){ return p1->getQualificationTime() < p2->getQualificationTime();});

    float i = 0.0;
    for_each(l_teams.begin(), l_teams.end(),
             [&](auto p){ l_vecSeq.push_back(make_pair(p, (i++) + this -> calcTime(p->getCar(),p_track)));});

    sort(l_vecSeq.begin(), l_vecSeq.end(),
         [](auto p1, auto p2){ return p1.second < p2.second;});

    auto l_resultIter = l_vecSeq.begin();
    for(auto l_iter = l_vecSeq.begin(); l_iter != l_vecSeq.end();)
    {
        l_resultIter = find_if(l_iter, l_vecSeq.end(),[&](auto p){return p.second != l_iter->second;});
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

    for_each(l_vecSeq.begin(), l_vecSeq.end(),
                 [&](auto& p){ p.second += this -> calcTime(p.first->getCar(),p_track);});

    vector<int> l_res;
    for_each(l_vecSeq.begin(), l_vecSeq.end(),
             [&](auto p){ l_res.push_back(p.first->getId());});

    return move(l_res);
}

bool Race::validate(const ICar* p_car)
{
    return (100 == p_car -> statusOfTire() and
             100 == p_car -> statusOfEngine() and
             100 == p_car -> statusOfSuspension());
}

float Race::calcTime(const ICar* p_car, const ITrack& p_track)
{
    int l_timeOfLength = (EngineQuality::High == p_car -> qualityOfEngine()) ? 5 : 6;
    float l_timeOfTurns = (Handling::Good == p_car -> handling()) ? 0.5 : 1.5;

    return p_track.getLength() / 100 * l_timeOfLength + p_track.getTurns()* l_timeOfTurns;
}

