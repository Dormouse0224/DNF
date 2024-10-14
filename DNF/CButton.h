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

public:
    virtual void Render() override;         // 오브젝트를 그리기

    void SetAction(CAlbumPlayer* _Scene, BtnState _BtnState);
};


