#pragma once
#include "CState.h"

class CAlbumPlayer;
class CDummy;

class CPlayer_Death :
    public CState
{
public:
    CPlayer_Death(wstring _name);
    ~CPlayer_Death();

private:
    float m_Countdown;
    CDummy* m_CountdownObj;
    CAlbumPlayer* m_CountdownTex;

public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();
};

