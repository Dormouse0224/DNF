#pragma once
#include "CLevel.h"

class CTexture;

class CLevel_Edit :
    public CLevel
{
    public:
    CLevel_Edit();
    ~CLevel_Edit();

private:
    CTexture* m_PreviewTexture;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;
    void SetPreviewTexture(CTexture* _pTexture) { m_PreviewTexture = _pTexture; }
};

