#pragma once
#include "CState.h"
class CMonster_Idle :
    public CState
{
public:
    CMonster_Idle(wstring _name);
    ~CMonster_Idle();

private:
    float m_Timer;


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

