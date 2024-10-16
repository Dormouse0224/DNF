#pragma once
#include "CLevel.h"

class CTexture;
class CAlbumPlayer;
class CBackground;

class CLevel_Edit :
    public CLevel
{
    public:
    CLevel_Edit();
    ~CLevel_Edit();

private:
    CTexture* m_PreviewTexture;
    HWND m_hAlbumViewerWnd;
    HWND m_hCreateAlbum;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;
    void LoadNPKCallback();
    void CreateAlbumCallback();
    void SetPreviewTexture(CTexture* _pTexture) { m_PreviewTexture = _pTexture; }
};

