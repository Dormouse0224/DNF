#pragma once
#include "CLevel.h"

class CBackground;
class CMonster;
class CNPC;
class CSound;
class CObj;

class CStage :
    public CLevel
{
public:
    CStage(wstring _name);
    ~CStage();

protected:
    StageInfo* m_StageInfo;
    CSound* m_BGM;


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;

    StageInfo* GetStageInfo() { return m_StageInfo; }
};

