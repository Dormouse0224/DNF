#pragma once
#include "CState.h"
class CMonster_Hurt :
    public CState
{
public:
    CMonster_Hurt(wstring _name);
    ~CMonster_Hurt();

private:
    float time;


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

