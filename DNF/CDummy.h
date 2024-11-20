#pragma once
#include "CObj.h"
class CDummy :
    public CObj
{
public:
    CDummy(wstring _name);
    ~CDummy();

private:
    float m_SuicideTimer;
    bool m_bTimerOn;

public:
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);


    virtual void Begin();       // ó�� ������ �� �� ��
    virtual void Tick();    // �ݺ������� �� ��
    virtual void Render();      // ������Ʈ�� �׸���

    void SetSuicideTimer(float _f) { m_SuicideTimer = _f; }
};

