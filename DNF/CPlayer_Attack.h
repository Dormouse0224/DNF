#pragma once
#include "CState.h"
class CPlayer_Attack :
    public CState
{
public:
    CPlayer_Attack(wstring _name);
    ~CPlayer_Attack();

private:


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

