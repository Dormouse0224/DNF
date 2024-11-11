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
    // 충돌 함수
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);

    virtual void Begin();       // 처음 시작할 때 할 일
    virtual void Tick();        // 반복적으로 할 일
    virtual void Render();      // 오브젝트를 그리기

    void AddAnimation(CAlbumPlayer* _AP, LargoState _st);
    vector<CAlbumPlayer*>& GetAnimationVec(LargoState _st) { return m_Animation[(int)_st]; }
    bool IsIncomeFin() { return m_bIncomeFin; }
    void SetAttackState(CLargo_Attack* _AtkState) { m_AttackState = _AtkState; }
};

