#pragma once
#include "CStage.h"

class CObj;

class CS01 :
    public CStage
{
public:
    CS01();
    ~CS01();

private:
    float m_Duration;
    vector<CObj*> m_WaitList;
    int m_toDeleteId;

    bool m_PlayerIncome;
    bool m_scene01;


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;
};

