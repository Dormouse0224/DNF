#pragma once
#include "CObj.h"

enum class AoEType
{
    Circle,
    Donut,
    Rect,
    ScreenAtk,
    ScreenSlash,


    END
};

enum class ScreenEffect
{
    SlashStart,
    SlashLoop,
    SlashEnd,
    SignalLoop,
    Crash,

    END
};

enum class RiseExplosion
{
    Signal,
    Bottom,
    Top,

    END
};


class CAoE :
    public CObj
{
public:
    CAoE(wstring _name);
    ~CAoE();

private:
    AoEType     m_Type;
    Vec2D       m_Pos;
    int         m_Param1;
    int         m_Param2;
    float       m_CastTime;
    bool        m_IsVert;

    float       m_Timer;
    bool        m_bInit;
    bool        m_IsCastFin;
    bool        m_IsEffectFin;

    CAlbumPlayer* m_RingSlash;
    CAlbumPlayer* m_ScreenEffect[(int)ScreenEffect::END];
    vector<CAlbumPlayer*> m_RiseExplosion[(int)RiseExplosion::END];

public:
    virtual void Begin();       // 처음 시작할 때 할 일
    virtual void Tick();    // 반복적으로 할 일
    virtual void Render();      // 오브젝트를 그리기

    void SetInfo(AoEType _type, Vec2D _pos, int _param1, int _param2, float _castTime, bool _isVert);
    bool IsEffectFin() { return m_IsEffectFin; }
};

