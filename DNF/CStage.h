#pragma once
#include "CLevel.h"

class CBackground;
class CMonster;
class CNPC;
class CSound;
class CObj;
class CAlbumPlayer;
class CPortal;

class CStage :
    public CLevel
{
public:
    CStage(wstring _name);
    ~CStage();

protected:
    StageInfo* m_StageInfo;
    CSound* m_BGM;
    int m_UpperBound;

    bool m_bStageClear;
    float m_ClearTimer;

    CAlbumPlayer* m_Effect;
    CObj* m_EffectOwner;

    vector<CPortal*> m_PortalVec;


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;

    StageInfo* GetStageInfo() { return m_StageInfo; }
    void SetStageInfo(StageInfo* _info) { m_StageInfo = _info; }
};

