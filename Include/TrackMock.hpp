#include "ITrack.hpp"

class TrackMock : public ITrack
{
public:
    MOCK_CONST_METHOD0(getLength, int());
    MOCK_CONST_METHOD0(getTurns, int());
};

