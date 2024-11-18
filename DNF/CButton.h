#pragma once
#include "CUI.h"

class CAlbumPlayer;

enum class BtnState
{
    IDLE,
    CURSOR_ON,
    PRESSED,

    END
};


class CButton :
    public CUI
{
public:
    CButton(wstring _Name);
    ~CButton();

private:
    CAlbumPlayer* m_Action[(int)BtnState::END];

    CALLBACK_0 m_ClickCallback0;
    CALLBACK_1 m_ClickCallback1;
    DWORD_PTR m_ClickCallback1_param;

    CBase* m_DelegateObj;
    DELEGATE_0 m_ClickDelegate0;
    DELEGATE_1 m_ClickDelegate1;
    DWORD_PTR m_ClickDelegate1_param;

    // 버튼쪽에서는 AddComponent_Btn 이 쓰일 것이기 때문에 함수 접근을 제한
    //virtual void AddComponent(CComponent* _Component) override { CObj::AddComponent(_Component); }

public:
    virtual void Render_UI() override;         // 오브젝트를 그리기
    virtual void Tick_UI() {};
    void AddComponent_Btn(CAlbumPlayer* _Scene, BtnState _BtnState);
    virtual void MouseLBtnClikced();
    void SetCallback0(CALLBACK_0 _CallbackFunc) { m_ClickCallback0 = _CallbackFunc; }
    void SetCallback1(CALLBACK_1 _CallbackFunc, DWORD_PTR _param) { m_ClickCallback1 = _CallbackFunc; m_ClickCallback1_param = _param; }
    void SetDelegate0(CBase* _DelegObj, DELEGATE_0 _DelegFunc) { m_DelegateObj = _DelegObj; m_ClickDelegate0 = _DelegFunc; }
    void SetDelegate1(CBase* _DelegObj, DELEGATE_1 _DelegFunc, DWORD_PTR _param) { m_DelegateObj = _DelegObj; m_ClickDelegate1 = _DelegFunc; m_ClickDelegate1_param = _param; }
};


