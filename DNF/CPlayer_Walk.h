#pragma once
#include "CState.h"
#include "CKeyMgr.h"

class CPlayer_Walk :
    public CState
{
public:
    CPlayer_Walk(wstring _name);
    ~CPlayer_Walk();

private:
    float m_Speed = 200.f;

public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

