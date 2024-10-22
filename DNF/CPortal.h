#pragma once
#include "CObj.h"

class CPortal :
    public CObj
{
public:
    CPortal(wstring _name);
    ~CPortal();

private:



public:

    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);

    virtual void Begin();       // 처음 시작할 때 할 일
    virtual void Tick();        // 반복적으로 할 일
};

