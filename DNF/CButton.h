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

    bool m_IsPressed;   // ��ư ���� �ȿ��� ���콺 ���� Ű�� ���� ���� ����




public:
    virtual void Begin() override;          // ó�� ������ �� �� ��
    virtual void Tick() override;           // �ݺ������� �� ��
    virtual void Render() override;         // ������Ʈ�� �׸���

    void SetAction(CAlbumPlayer* _Scene, BtnState _BtnState);

    virtual void Click() = 0;
};


