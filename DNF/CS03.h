#pragma once
#include "CStage.h"
#include "CAoE.h"

class CLargo;
class CSea;

class CS03 :
    public CStage
{
public:
    CS03();
    ~CS03();

private:
    CLargo* m_Largo;
    vector<CAoE*> m_AoEVec;
    CSea* m_Sea[2];
    int m_SeaGap;
    CDummy* m_PlayerYogiGauge;
    CAlbumPlayer* m_PlayerYogi;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;

    void AddAoE(AoEType _type, Vec2D _pos, int _param1, int _param2, float _castTime, bool _isVert = false);
};

