#pragma once
#include "CUI.h"
class CWindow :
    public CUI
{
public:
    CWindow(wstring _name);
    ~CWindow();

private:
    Color m_WindowColor;
    Vec2D m_PrevCursorPos;
    bool m_bMovalbe;

    Vec2D m_BeginCursorPos;
    Vec2D m_EndCursorPos;


    CBase* m_DelegateObj;
    DELEGATE_1 m_RenderDelegate1;
    DWORD_PTR m_RenderDelegate1_param;

public:
    virtual void Tick_UI() override;
    virtual void Render_UI() override;

    // 마우스 왼쪽 버튼이 눌렸을 때
    virtual void MouseLBtnDown();

    // 해당 UI 가 눌렸고, 다시 UI 위에서 버튼이 떼졌을 때 클릭으로 판정한다.
    virtual void MouseLBtnClikced() {}

    // 해당 UI 가 눌리진 않았지만, UI 위에서 버튼이 떼졌을 때, Drag Drop 될 때 활용
    virtual void MouseReleased();


    void SetMovalbe(bool _b) { m_bMovalbe = _b; }
    void SetWindowColor(Color _c) { m_WindowColor = _c; }
    void SetRenderDelegate1(CBase* _DelegObj, DELEGATE_1 _DelegFunc, DWORD_PTR _param) { m_DelegateObj = _DelegObj; m_RenderDelegate1 = _DelegFunc; m_RenderDelegate1_param = _param; }
};

