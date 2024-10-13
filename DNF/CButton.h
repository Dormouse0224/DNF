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

    bool m_IsPressed;   // 버튼 영역 안에서 마우스 좌측 키가 눌린 적이 있음




public:
    virtual void Begin() override;          // 처음 시작할 때 할 일
    virtual void Tick() override;           // 반복적으로 할 일
    virtual void Render() override;         // 오브젝트를 그리기

    void SetAction(CAlbumPlayer* _Scene, BtnState _BtnState);

    virtual void Click() = 0;
};


