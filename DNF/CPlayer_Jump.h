#pragma once
#include "CState.h"
class CPlayer_Jump :
    public CState
{
public:
    CPlayer_Jump(wstring _name);
    ~CPlayer_Jump();

private:
    float m_Speed = 150.f;


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

