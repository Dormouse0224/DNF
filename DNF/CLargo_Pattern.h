#pragma once
#include "CState.h"

class CLargo_Pattern :
    public CState
{
public:
    CLargo_Pattern();
    ~CLargo_Pattern();

private:
    float m_PatternTimer;


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

