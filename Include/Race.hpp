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
    bool validate(const ICar*);
    float calcTime(const ICar*, const ITrack&);
};
