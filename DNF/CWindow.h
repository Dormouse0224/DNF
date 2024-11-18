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

    // ���콺 ���� ��ư�� ������ ��
    virtual void MouseLBtnDown();

    // �ش� UI �� ���Ȱ�, �ٽ� UI ������ ��ư�� ������ �� Ŭ������ �����Ѵ�.
    virtual void MouseLBtnClikced() {}

    // �ش� UI �� ������ �ʾ�����, UI ������ ��ư�� ������ ��, Drag Drop �� �� Ȱ��
    virtual void MouseReleased();


    void SetMovalbe(bool _b) { m_bMovalbe = _b; }
    void SetWindowColor(Color _c) { m_WindowColor = _c; }
    void SetRenderDelegate1(CBase* _DelegObj, DELEGATE_1 _DelegFunc, DWORD_PTR _param) { m_DelegateObj = _DelegObj; m_RenderDelegate1 = _DelegFunc; m_RenderDelegate1_param = _param; }
};

