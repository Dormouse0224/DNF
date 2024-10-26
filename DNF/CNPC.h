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

public:
    // �浹 �Լ�
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);


    virtual void Begin();       // ó�� ������ �� �� ��
    virtual void Tick();    // �ݺ������� �� ��
    virtual void Render();      // ������Ʈ�� �׸���
};

