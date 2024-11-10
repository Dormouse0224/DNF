#pragma once
#include "CObj.h"

class CAlbumPlayer;
class CSound;

enum class AoEEffLayer
{
    a,
    b,
    c,

    END
};

class CPlayerAoE :
    public CObj
{
public:
    CPlayerAoE(wstring _name, float _Radius);
    ~CPlayerAoE();

private:
    CAlbumPlayer* m_EffAni[(int)AoEEffLayer::END];
    int m_ATK;
    bool m_bAttacked;
    float m_Radius;
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
    void SetEffSound(CSound* _sound) { m_EffSound = _sound; }
    void SetEffAni(CAlbumPlayer* _AP, AoEEffLayer _layer) { m_EffAni[(int)_layer] = _AP; }

    void CheckEnd();
};

