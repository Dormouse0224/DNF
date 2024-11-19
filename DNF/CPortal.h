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
    int m_DetectRange = 50;


public:

    virtual void Overlap(CCollider* _Self, CCollider* _Other);

    virtual void Begin();       // ó�� ������ �� �� ��
    virtual void Tick();        // �ݺ������� �� ��
    virtual void Render();      // ������Ʈ�� �׸���

    void SetDest(CLevel* _Dest) { m_Dest = _Dest; }
    void AddDeactiveAnimation(CAlbumPlayer* _AlbumPlayer) { m_DeactiveAnimation.push_back(_AlbumPlayer); }
    void ToggleActive() { m_bActive = !m_bActive; }
    void SetActive(bool _b) { m_bActive = _b; }
};

