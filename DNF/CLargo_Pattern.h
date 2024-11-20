#pragma once
#include "CState.h"

class CDummy;

class CLargo_Pattern :
    public CState
{
public:
    CLargo_Pattern();
    ~CLargo_Pattern();

private:
    float m_PatternTimer;
    bool m_bClear;
    int m_VoiceNum;
    CDummy* m_Effect;

    CAlbumPlayer* m_IdleAnimation;
    CAlbumPlayer* m_IdleAnimation1;

    bool m_bStop;
    Vec2D m_BarPos;


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();

    void Render();
};

