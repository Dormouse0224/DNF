#pragma once
#include "CObj.h"

class CAlbumPlayer;
class CLargo_Attack;
class CDummy;

class CLargo :
    public CObj
{
public:
    CLargo();
    ~CLargo();

private:
    vector<CAlbumPlayer*> m_Animation[(int)LargoState::END];
    bool m_bIncomeFin;
    CLargo_Attack* m_AttackState;

    CDummy* m_HUD;
    CAlbumPlayer* m_HPTex;


public:
    // �浹 �Լ�
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);

    virtual void Begin();       // ó�� ������ �� �� ��
    virtual void Tick();        // �ݺ������� �� ��
    virtual void Render();      // ������Ʈ�� �׸���

    void AddAnimation(CAlbumPlayer* _AP, LargoState _st);
    vector<CAlbumPlayer*>& GetAnimationVec(LargoState _st) { return m_Animation[(int)_st]; }
    bool IsIncomeFin() { return m_bIncomeFin; }
    void SetAttackState(CLargo_Attack* _AtkState) { m_AttackState = _AtkState; }
};

