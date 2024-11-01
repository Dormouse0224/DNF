#pragma once
#include "CState.h"
class CPlayer_Run :
    public CState
{
public:
    CPlayer_Run(wstring _name);
    ~CPlayer_Run();

private:
    float m_Speed = 250.f;


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

