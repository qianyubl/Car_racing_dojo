#pragma once

class ITrack
{
public:
    virtual ~ITrack(){}
    virtual int getLength() const = 0;
    virtual int getTurns() const = 0;
};
