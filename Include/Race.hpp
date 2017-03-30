#pragma once

#include <vector>
#include <functional>

class ICar;
class ITrack;
class ITeam;

using namespace std;

class Race
{
public:
    vector<int> run(const vector<reference_wrapper<ITeam>>&, const ITrack&);
    bool validate(const ICar&);
    float calcTime(const ICar&, const ITrack&);
};
