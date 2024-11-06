#pragma once
#include "CStage.h"
class CCliff :
    public CStage
{
public:
    CCliff();
    ~CCliff();

private:
    CAlbumPlayer* m_Cliff[2];
    float m_Speed = 2400.f;
    float m_Duration;
    bool m_fadeout;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;
};

