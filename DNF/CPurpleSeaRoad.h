#pragma once
#include "CStage.h"

class CPurpleSeaRoad :
    public CStage
{
public:
    CPurpleSeaRoad();
    ~CPurpleSeaRoad();


private:
    float m_Timer;


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;
};

