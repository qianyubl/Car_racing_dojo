#pragma once

enum class EngineQuality
{
    High,
    Low
};

enum class Handling
{
    Good,
    Bad
};

class ICar
{
public:
    virtual ~ICar(){}
    virtual int statusOfTire() const = 0;
    virtual int statusOfEngine() const = 0;
    virtual int statusOfSuspension() const = 0;
    virtual EngineQuality qualityOfEngine() const = 0;
    virtual Handling handling() const = 0;
};
