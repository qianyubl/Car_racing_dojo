#pragma once

#include <vector>

using namespace std;

class ITeam;
class Track;

class Race
{
public:
    vector<int> run(const vector<ITeam *>& p_teams, const Track& p_track);
};
