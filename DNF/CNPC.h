#pragma once
#include "CObj.h"

typedef void (*NPCReactCallback0)(void);

class CNPC :
    public CObj
{
public:
    CNPC(wstring _name);
    ~CNPC();

private:
    NPCReactCallback0 m_reactCallback;
    wstring m_IdleAni;

public:
    // 충돌 함수
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);


    virtual void Begin();       // 처음 시작할 때 할 일
    virtual void Tick();    // 반복적으로 할 일
    virtual void Render();      // 오브젝트를 그리기


    wstring GetIdleAni() { return m_IdleAni; }
    void SetIdleAni(wstring _animation) { m_IdleAni = _animation; }
};

