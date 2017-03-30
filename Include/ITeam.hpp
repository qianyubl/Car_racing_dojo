#pragma once

class ICar;

class ITeam
{
public:
    virtual ~ITeam(){}
    virtual ICar* getCar() const = 0;
    virtual int getId() const = 0;
};
