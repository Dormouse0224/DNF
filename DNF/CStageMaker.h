#pragma once
#include "CLevel.h"

class CBackground;
class CMonster;
class CNPC;

class CStageMaker :
    public CLevel
{
public:
    CStageMaker();
    ~CStageMaker();

private:
    CBackground* m_Background;
    vector<CMonster*> m_MonsterVec;
    vector<CNPC*> m_NPCVec;


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;


};

