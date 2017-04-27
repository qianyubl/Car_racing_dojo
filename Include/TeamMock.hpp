#include "ITeam.hpp"

class TeamMock : public ITeam
{
public:

    MOCK_CONST_METHOD0(getCar, const ICar*());
    MOCK_CONST_METHOD0(getId, int());
    MOCK_CONST_METHOD0(getQualificationTime, float());

};
