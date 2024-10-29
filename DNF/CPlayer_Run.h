#pragma once
#include "CState.h"
class CPlayer_Run :
    public CState
{
public:
    CPlayer_Run(wstring _name);
    ~CPlayer_Run();

private:
    bool m_bHorInit;
    bool m_bVertInit;
    int m_Speed = 250;


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

