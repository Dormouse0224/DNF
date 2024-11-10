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
    // 충돌 함수
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);


    virtual void Begin();       // 처음 시작할 때 할 일
    virtual void Tick();        // 반복적으로 할 일
    virtual void Render();      // 오브젝트를 그리기

    void SetATK(int _atk) { m_ATK = _atk; }
    void SetEffSound(CSound* _sound) { m_EffSound = _sound; }
    void SetEffAni(CAlbumPlayer* _AP, AoEEffLayer _layer) { m_EffAni[(int)_layer] = _AP; }

    void CheckEnd();
};

