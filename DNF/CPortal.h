#pragma once
#include "CObj.h"

class CAlbumPlayer;
class CCollider;

class CPortal :
    public CObj
{
public:
    CPortal(wstring _name);
    ~CPortal();

private:
    bool m_bActive;
    CLevel* m_Dest;
    vector<CAlbumPlayer*> m_DeactiveAnimation;


public:

    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);

    virtual void Begin();       // 처음 시작할 때 할 일
    virtual void Tick();        // 반복적으로 할 일
    virtual void Render();      // 오브젝트를 그리기

    void SetDest(CLevel* _Dest) { m_Dest = _Dest; }
    void AddDeactiveAnimation(CAlbumPlayer* _AlbumPlayer) { m_DeactiveAnimation.push_back(_AlbumPlayer); }
    void ToggleActive() { m_bActive = !m_bActive; }
};

