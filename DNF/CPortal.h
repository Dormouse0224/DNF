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

    virtual void Begin();       // ó�� ������ �� �� ��
    virtual void Tick();        // �ݺ������� �� ��
};

