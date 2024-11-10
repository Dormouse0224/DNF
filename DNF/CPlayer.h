#pragma once
#include "CObj.h"

class CPlayer_Attack;

class CPlayer :
    public CObj
{
public:
    CPlayer();
    ~CPlayer();

private:
    vector<CAlbumPlayer*> m_Animation[(int)PlayerState::END];
    float m_YogiGauge;
    bool m_Setup;
    vector<CAlbumPlayer*> m_Avatar[(int)PlayerState::END];
    string m_CurAvatarCode[(int)AvatarParts::END];
    string m_PrevAvatarCode[(int)AvatarParts::END];
    CPlayer_Attack* m_AttackState;


public:
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);


    virtual void Begin();       // 처음 시작할 때 할 일
    virtual void Tick();        // 반복적으로 할 일
    virtual void Render();      // 오브젝트를 그리기

    void AddAnimation(PlayerState _state, CAlbumPlayer* _animation);

    float GetYogiGauge() { return m_YogiGauge; }
    float SetYogiGauge(float _val) { m_YogiGauge = _val; }
    void AddYogiGauge(float _val) { m_YogiGauge  = min(m_YogiGauge + _val, 110); }

    bool GetSetup() { return m_Setup; }
    void SetSetup(bool _b) { m_Setup = _b; }

    string* GetCurAvatarCode() { return m_CurAvatarCode; }

    bool CheckAvatarCodeChange();

    CPlayer_Attack* GetAttackState() { return m_AttackState; }
};

