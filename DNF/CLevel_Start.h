#pragma once
#include "CLevel.h"
class CLevel_Start :
    public CLevel
{
public:
    CLevel_Start();
    ~CLevel_Start();

private:


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;

private:
    void GameStartBtnCallback();
    void TextureEditBtnCallback();
    void DungeonMakerBtnCallback();
};

