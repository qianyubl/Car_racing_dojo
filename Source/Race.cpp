#include "Race.hpp"
#include "ITeam.hpp"
#include "Track.hpp"
#include <utility>
#include <algorithm>

vector<int> Race::run(const vector<ITeam *>& p_teams, const Track& p_track)
{
    vector<pair<int, int>> l_vecSeq;

    for_each(p_teams.begin(), p_teams.end(),
             [&](auto p){ l_vecSeq.push_back(make_pair(p->getId(), p->getTotalTime()));});

    sort(l_vecSeq.begin(), l_vecSeq.end(),
         [](auto p1, auto p2){ return p1.second < p2.second;});

    vector<int> l_res;
    for_each(l_vecSeq.begin(), l_vecSeq.end(),
             [&](auto p){ l_res.push_back(p.first);});

    return move(l_res);
}