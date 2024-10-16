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

    // ��ư�ʿ����� AddComponent_Btn �� ���� ���̱� ������ �Լ� ������ ����
    virtual void AddComponent(CComponent* _Component) { CObj::AddComponent(_Component); }

public:
    virtual void Render() override;         // ������Ʈ�� �׸���

    void AddComponent_Btn(CAlbumPlayer* _Scene, BtnState _BtnState);
};


