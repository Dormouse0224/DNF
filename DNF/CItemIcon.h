#pragma once
#include "CUI.h"

class CItemIcon :
    public CUI
{
public:
    CItemIcon(wstring _name, int _idx);
    ~CItemIcon();

private:
    Vec2D m_PrevCursorPos;
    Vec2D m_BeginCursorPos;
    Vec2D m_EndCursorPos;

    ItemInfo* m_ItemInfo;
    int m_InventoryIndex;


public:
    virtual void Tick_UI() override;
    virtual void Render_UI() override;

    // ���콺 ���� ��ư�� ������ ��
    virtual void MouseLBtnDown();

    // �ش� UI �� ���Ȱ�, �ٽ� UI ������ ��ư�� ������ �� Ŭ������ �����Ѵ�.
    virtual void MouseLBtnClikced();

    // �ش� UI �� ������ �ʾ�����, UI ������ ��ư�� ������ ��, Drag Drop �� �� Ȱ��
    virtual void MouseReleased();

    void DragEnd();
    void SetItemInfo(ItemInfo* _info);
    void SetEquipIconPos(int _idx);
    void SetAvatarIconPos(int _idx);
};

