#pragma once
#include "CState.h"
class CLargo_Death :
    public CState
{
public:
    CLargo_Death();
    ~CLargo_Death();

private:


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

