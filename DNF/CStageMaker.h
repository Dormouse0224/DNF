#pragma once
#include "CLevel.h"

class CBackground;
class CMonster;
class CNPC;
class CSound;
class CObj;

class CStageMaker :
    public CLevel
{
public:
    CStageMaker();
    ~CStageMaker();

private:
    StageInfo* m_StageInfo;
    CBackground* m_Background;
    CSound* m_BGM;
    vector<CObj*> m_StageObject;


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;


};

