#pragma once
#include "CStage.h"
class CS03 :
    public CStage
{
public:
    CS03();
    ~CS03();

private:


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;
};

