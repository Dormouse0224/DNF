#pragma once
#include "CLevel.h"
class CLevel_Edit :
    public CLevel
{
    public:
    CLevel_Edit();
    ~CLevel_Edit();

private:


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;
};

