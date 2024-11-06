#pragma once
#include "CObj.h"
class CDummy :
    public CObj
{
public:
    CDummy(wstring _name);
    ~CDummy();

private:


public:
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);


    virtual void Begin();       // 처음 시작할 때 할 일
    virtual void Tick();    // 반복적으로 할 일
    virtual void Render();      // 오브젝트를 그리기

};

