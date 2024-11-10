#pragma once
#include "CObj.h"

class CAlbumPlayer;
class CSound;

enum class SkillEffLayer
{
    a,
    b,
    c,

    END
};

class CPlayerSkill :
    public CObj
{
public:
    CPlayerSkill(wstring _name, float _Lifetime);
    ~CPlayerSkill();

private:
    CAlbumPlayer* m_EffAni[(int)SkillEffLayer::END];
    int m_ATK;
    int m_DotATK;
    int m_FinATK;
    float m_DotDuration;
    float m_DotTimer;
    float m_Lifetime;
    CSound* m_EffSound;
    bool m_bSoundPlayed;


public:
    // �浹 �Լ�
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);


    virtual void Begin();       // ó�� ������ �� �� ��
    virtual void Tick();        // �ݺ������� �� ��
    virtual void Render();      // ������Ʈ�� �׸���

    void SetATK(int _atk) { m_ATK = _atk; }
    void SetDotATK(int _atk) { m_DotATK = _atk; }
    void SetFinATK(int _atk) { m_FinATK = _atk; }
    void SetDotDuration(float _dur) { m_DotDuration = _dur; }
    void SetEffSound(CSound* _sound) { m_EffSound = _sound; }
    void SetEffAni(CAlbumPlayer* _AP, SkillEffLayer _layer) { m_EffAni[(int)_layer] = _AP; }
};

