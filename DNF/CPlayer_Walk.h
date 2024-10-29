#pragma once
#include "CState.h"
class CPlayer_Walk :
    public CState
{
public:
    CPlayer_Walk(wstring _name);
    ~CPlayer_Walk();

private:
    bool m_bHorInit;
    bool m_bVertInit;
    int m_Speed = 150;

public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

