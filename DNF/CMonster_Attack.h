#pragma once
#include "CState.h"
class CMonster_Attack :
    public CState
{
public:
    CMonster_Attack(wstring _name);
    ~CMonster_Attack();

private:



public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

