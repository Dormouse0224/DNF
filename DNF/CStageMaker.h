#pragma once
#include "CStage.h"

class CObj;

class CStageMaker :
    public CStage
{
public:
    CStageMaker();
    ~CStageMaker();

private:
    HWND m_hControlPanel;
    HWND m_hSelectedObj;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;

};

