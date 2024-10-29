#pragma once
#include "CObj.h"


class CPlayer :
    public CObj
{
public:
    CPlayer();
    ~CPlayer();

private:
    vector<CAlbumPlayer*> m_Animation[(int)PlayerState::END];


public:
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);


    virtual void Begin();       // ó�� ������ �� �� ��
    virtual void Tick();        // �ݺ������� �� ��
    virtual void Render();      // ������Ʈ�� �׸���

    void AddAnimation(PlayerState _state, CAlbumPlayer* _animation);
};

