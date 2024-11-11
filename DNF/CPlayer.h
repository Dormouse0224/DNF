#pragma once
#include "CObj.h"

class CPlayer_Attack;
class CDummy;

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

    CDummy* m_HUD;
    CAlbumPlayer* m_HPTex;
    CAlbumPlayer* m_MPTex;

    float m_HPRegen;
    float m_MPRegen;


public:
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other) override;
    virtual void Overlap(CCollider* _Self, CCollider* _Other) override;
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other) override;

    virtual void GiveDamage(int _dmg) override;

    virtual void Begin() override;       // 처음 시작할 때 할 일
    virtual void Tick() override;        // 반복적으로 할 일
    virtual void Render() override;      // 오브젝트를 그리기

    void AddAnimation(PlayerState _state, CAlbumPlayer* _animation);
    vector<CAlbumPlayer*>& GetAnimationVec(PlayerState _st) { return m_Animation[(int)_st]; }
    vector<CAlbumPlayer*>& GetAvatarVec(PlayerState _st) { return m_Avatar[(int)_st]; }

    float GetYogiGauge() { return m_YogiGauge; }
    float SetYogiGauge(float _val) { m_YogiGauge = _val; }
    void AddYogiGauge(float _val) { m_YogiGauge  = min(m_YogiGauge + _val, 110); }

    bool GetSetup() { return m_Setup; }
    void SetSetup(bool _b) { m_Setup = _b; }

    string* GetCurAvatarCode() { return m_CurAvatarCode; }

    bool CheckAvatarCodeChange();

    CPlayer_Attack* GetAttackState() { return m_AttackState; }

};

