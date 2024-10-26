#pragma once
#include "CComponent.h"


class CSticker :
    public CComponent
{
    friend class CStickerMgr;
public:
    CSticker(wstring _name);
    ~CSticker();

private:
    bool m_bCursorOn;
    bool m_IsPressed;

public:
    // ������ �ȿ��� ������Ʈ�� �� ��
    virtual void FinalTick();

    bool IsCursorOn() { return m_bCursorOn; }
    void SetPressed(bool _state) { m_IsPressed = _state; }
    bool IsPressed() { return m_IsPressed; }

};

