#pragma once
#include "CButton.h"
class CButton_LoadNPK :
    public CButton
{
public:
    CButton_LoadNPK(wstring _name);
    ~CButton_LoadNPK();

private:
    HWND m_hAlbumViewerWnd;

public:
    virtual void MouseLBtnClikced() override;

    virtual void Tick_UI() {};
};

