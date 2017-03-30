#include "ITeam.hpp"

class TeamMock : public ITeam
{
public:

    MOCK_CONST_METHOD0(getTotalTime, int());
    MOCK_CONST_METHOD0(getId, int());

};
