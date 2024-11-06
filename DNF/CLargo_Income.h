#pragma once
#include "CState.h"
class CLargo_Income :
    public CState
{
public:
    CLargo_Income();
    ~CLargo_Income();

private:


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();

};

