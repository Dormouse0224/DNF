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
    Vec2D m_UIPos;          // ���� UI ���� ����� ��ġ (�ֻ��� UI �� ��� ī�޶� ���� ���� ��ġ)

    CUI* m_Parent;
    vector<CUI*> m_ChildVec;

    bool m_PrevPressed;     // ���� ������ ���� ����
    bool m_IsPressed;       // ���� �ȿ��� ���콺 ���� Ű�� ���� ���� ����

    ItemType m_UIItemType;
    InventoryType m_UIInventoryType;

public:
    virtual void Tick() override;
    virtual void Tick_UI() = 0;

    virtual void Render() override;
    virtual void Render_UI();

    virtual void FinalTick() override;
    virtual void FinalTick_UI();

    // ���콺 ���� ��ư�� ������ ��
    virtual void MouseLBtnDown() {}

    // �ش� UI �� ���Ȱ�, �ٽ� UI ������ ��ư�� ������ �� Ŭ������ �����Ѵ�.
    virtual void MouseLBtnClikced() {}

    // �ش� UI �� ������ �ʾ�����, UI ������ ��ư�� ������ ��, Drag Drop �� �� Ȱ��
    virtual void MouseReleased() {}

    void AddChild(CUI* _Child, wstring _name);
    bool IsCursorOn() { return m_IsCursorOn; }
    void SetUIPos(Vec2D _pos) 
    { 
        m_UIPos = _pos;
    }
    Vec2D GetUIPos() { return m_UIPos; }
    void SetParent(CUI* _p) { m_Parent = _p; }
    CUI* GetParent() { return m_Parent; }
    vector<CUI*>& GetChildVec() { return m_ChildVec; }
    ItemType GetUIItemType() { return m_UIItemType; }
    void SetUIItemType(ItemType _type) { m_UIItemType = _type; }
    InventoryType GetUIInventoryType() { return m_UIInventoryType; }
    void SetUIInventoryType(InventoryType _type) { m_UIInventoryType = _type; }
};

