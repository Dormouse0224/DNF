#pragma once
#include "CObj.h"
class CUI :
    public CObj
{
public:
    CUI(wstring _name);
    ~CUI();

private:
    bool m_IsCursorOn;

public:
    void Tick() override;
    bool IsCursorOn() { return m_IsCursorOn; }
};

