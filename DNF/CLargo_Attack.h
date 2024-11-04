#pragma once
#include "CState.h"
class CLargo_Attack :
    public CState
{
public:
    CLargo_Attack();
    ~CLargo_Attack();

private:


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();

    void Render();
};

