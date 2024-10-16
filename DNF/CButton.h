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

    // 버튼쪽에서는 AddComponent_Btn 이 쓰일 것이기 때문에 함수 접근을 제한
    virtual void AddComponent(CComponent* _Component) { CObj::AddComponent(_Component); }

public:
    virtual void Render() override;         // 오브젝트를 그리기

    void AddComponent_Btn(CAlbumPlayer* _Scene, BtnState _BtnState);
};


