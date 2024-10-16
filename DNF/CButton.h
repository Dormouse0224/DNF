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

    CALLBACK_0 m_ClickCallback;

    CBase* m_DelegateObj;
    DELEGATE_0 m_ClickDelegate;

    // ��ư�ʿ����� AddComponent_Btn �� ���� ���̱� ������ �Լ� ������ ����
    virtual void AddComponent(CComponent* _Component) { CObj::AddComponent(_Component); }

public:
    virtual void Render() override;         // ������Ʈ�� �׸���
    virtual void Tick_UI() {};
    void AddComponent_Btn(CAlbumPlayer* _Scene, BtnState _BtnState);
    virtual void MouseLBtnClikced();
    void SetCallback(CALLBACK_0 _CallbackFunc) { m_ClickCallback = _CallbackFunc; }
    void SetDelegate(CBase* _DelegObj, DELEGATE_0 _DelegFunc) { m_DelegateObj = _DelegObj; m_ClickDelegate = _DelegFunc; }
};


