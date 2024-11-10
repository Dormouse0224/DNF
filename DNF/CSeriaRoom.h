#pragma once
#include "CStage.h"
class CSeriaRoom :
    public CStage
{
public:
    CSeriaRoom();
    ~CSeriaRoom();

private:


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;

};

