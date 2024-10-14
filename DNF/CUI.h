#pragma once
#include "CObj.h"
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

    bool m_IsPressed;   // 영역 안에서 마우스 좌측 키가 눌린 적이 있음

public:
    virtual void Tick() override;
    virtual void Tick_UI() = 0;

    void AddChild(CUI* _Child, wstring _name);
    bool IsCursorOn() { return m_IsCursorOn; }

    // 마우스 왼쪽 버튼이 눌렸을 때
    virtual void MouseLBtnDown() {}

    // 해당 UI 가 눌렸고, 다시 UI 위에서 버튼이 떼졌을 때 클릭으로 판정한다.
    virtual void MouseLBtnClikced() {}

    // 해당 UI 가 눌리진 않았지만, UI 위에서 버튼이 떼졌을 때, Drag Drop 될 때 활용
    virtual void MouseReleased() {}
};

