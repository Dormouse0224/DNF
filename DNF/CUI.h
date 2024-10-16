#pragma once
#include "CObj.h"

typedef void (*CALLBACK_0)(void);
typedef void (*CALLBACK_1)(DWORD_PTR);

typedef void (CBase::* DELEGATE_0)(void);
typedef void (CBase::* DELEGATE_1)(DWORD_PTR);

class CUI :
    public CObj
{
    friend class CUIMgr;
public:
    CUI(wstring _name);
    ~CUI();

protected:
    bool m_IsCursorOn;
    Vec2D m_UIPos;

    CUI* m_Parent;
    vector<CUI*> m_ChildVec;

    bool m_IsPressed;   // ���� �ȿ��� ���콺 ���� Ű�� ���� ���� ����

public:
    virtual void Tick() override;
    virtual void Tick_UI() = 0;

    void AddChild(CUI* _Child, wstring _name);
    bool IsCursorOn() { return m_IsCursorOn; }

    // ���콺 ���� ��ư�� ������ ��
    virtual void MouseLBtnDown() {}

    // �ش� UI �� ���Ȱ�, �ٽ� UI ������ ��ư�� ������ �� Ŭ������ �����Ѵ�.
    virtual void MouseLBtnClikced() {}

    // �ش� UI �� ������ �ʾ�����, UI ������ ��ư�� ������ ��, Drag Drop �� �� Ȱ��
    virtual void MouseReleased() {}
};

