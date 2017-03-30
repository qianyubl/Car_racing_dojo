#include "ITeam.hpp"

class TeamMock : public ITeam
{
public:

    MOCK_CONST_METHOD0(getCar, ICar*());
    MOCK_CONST_METHOD0(getId, int());

};
