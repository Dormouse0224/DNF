#pragma once
#include "CObj.h"

class CAlbumPlayer;
class CSound;
class CArrow;

enum class ArrowEffLayer
{
    a,
    b,
    c,

    END
};

typedef void (CBase::* DELEGATE_1)(DWORD_PTR);

class CArrow :
    public CObj
{
public:
    CArrow(wstring _name, Vec2D _StartSpd, float _StartHeight, float _Lifetime);
    ~CArrow();

private:
    CAlbumPlayer* m_IdleAni;
    CAlbumPlayer* m_DeadAni[(int)ArrowEffLayer::END];
    CAlbumPlayer* m_DeadAniG[(int)ArrowEffLayer::END];
    bool m_bFin;
    int m_ATK;
    int m_PierceCount;
    float m_Lifetime;
    CSound* m_DeadSound;
    bool m_bSoundPlayed;
    CBase* m_DeadFuncObj;
    DELEGATE_1 m_DeadFunc;
    bool m_bFuncCalled;

public:
    // 충돌 함수
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);


    virtual void Begin();       // 처음 시작할 때 할 일
    virtual void Tick();        // 반복적으로 할 일
    virtual void Render();      // 오브젝트를 그리기

    void SetATK(int _atk) { m_ATK = _atk; }
    void SetPierceCount(int _count) { m_PierceCount = _count; }
    void SetDeadSound(CSound* _sound) { m_DeadSound = _sound; }

    void SetIdleAni(CAlbumPlayer* _AP) { m_IdleAni = _AP; }
    void SetDeadAni(CAlbumPlayer* _AP, ArrowEffLayer _layer) { m_DeadAni[(int)_layer] = _AP; }
    void SetDeadAniG(CAlbumPlayer* _AP, ArrowEffLayer _layer) { m_DeadAniG[(int)_layer] = _AP; }
    void SetDeadFunc(CBase* _obj, DELEGATE_1 _func) { m_DeadFuncObj = _obj; m_DeadFunc = _func; }
};

