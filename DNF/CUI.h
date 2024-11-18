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
    Vec2D m_UIPos;          // 상위 UI 기준 상대적 위치 (최상위 UI 의 경우 카메라 원점 기준 위치)

    CUI* m_Parent;
    vector<CUI*> m_ChildVec;

    bool m_PrevPressed;     // 이전 프레임 눌림 상태
    bool m_IsPressed;       // 영역 안에서 마우스 좌측 키가 눌린 적이 있음

    ItemType m_UIItemType;
    InventoryType m_UIInventoryType;

public:
    virtual void Tick() override;
    virtual void Tick_UI() = 0;

    virtual void Render() override;
    virtual void Render_UI();

    virtual void FinalTick() override;
    virtual void FinalTick_UI();

    // 마우스 왼쪽 버튼이 눌렸을 때
    virtual void MouseLBtnDown() {}

    // 해당 UI 가 눌렸고, 다시 UI 위에서 버튼이 떼졌을 때 클릭으로 판정한다.
    virtual void MouseLBtnClikced() {}

    // 해당 UI 가 눌리진 않았지만, UI 위에서 버튼이 떼졌을 때, Drag Drop 될 때 활용
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

