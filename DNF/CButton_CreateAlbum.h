#pragma once
#include "CButton.h"

class CButton_CreateAlbum :
    public CButton
{
public:
    CButton_CreateAlbum(wstring _name);
    ~CButton_CreateAlbum();

private:
    HWND m_hCreateAlbum;

public:
    virtual void MouseLBtnClikced() override;

    virtual void Tick_UI() {};


};

