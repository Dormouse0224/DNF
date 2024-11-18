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

    // 마우스 왼쪽 버튼이 눌렸을 때
    virtual void MouseLBtnDown();

    // 해당 UI 가 눌렸고, 다시 UI 위에서 버튼이 떼졌을 때 클릭으로 판정한다.
    virtual void MouseLBtnClikced();

    // 해당 UI 가 눌리진 않았지만, UI 위에서 버튼이 떼졌을 때, Drag Drop 될 때 활용
    virtual void MouseReleased();

    void DragEnd();
    void SetItemInfo(ItemInfo* _info);
    void SetEquipIconPos(int _idx);
    void SetAvatarIconPos(int _idx);
};

