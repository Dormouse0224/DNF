#pragma once
#include "CState.h"
class CMonster_Move :
    public CState
{
public:
    CMonster_Move(wstring _name);
    ~CMonster_Move();

private:



public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

