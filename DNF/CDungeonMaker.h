#pragma once
#include "CLevel.h"
class CDungeonMaker :
    public CLevel
{
public:
    CDungeonMaker(wstring _name);
    ~CDungeonMaker();

private:
    

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;

    void CreateStageCallback();

};

