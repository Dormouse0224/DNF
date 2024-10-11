#pragma once
#include "CObj.h"

class CAlbumPlayer;


class CButton :
    public CObj
{
public:
    CButton(wstring _Name);
    ~CButton();

private:
    CAlbumPlayer* m_Action[(int)BtnState::END];
    TextInfo m_Text[(int)BtnState::END];

    BtnState m_CurBtnState;
    BtnState m_PrevBtnState;

    bool m_IsBtnReleased;




public:
    virtual void Begin() override;       // 처음 시작할 때 할 일
    virtual void Tick() override;    // 반복적으로 할 일
    virtual void Render() override;      // 오브젝트를 그리기

    void SetAction(CAlbumPlayer* _Scene, BtnState _BtnState) { m_Action[(int)_BtnState] = _Scene; }
    void SetText(TextInfo _Text, BtnState _BtnState) { m_Text[(int)_BtnState] = _Text; }

    bool IsBtnReleased() { return m_IsBtnReleased; }
    void SetFalse() { m_IsBtnReleased = false; }
};

