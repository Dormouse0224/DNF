#pragma once
#include "CState.h"

class CPlayer_Idle :
    public CState
{
public:
    CPlayer_Idle(wstring _name);
    ~CPlayer_Idle();

private:


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

