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
    CAlbumPlayer* m_PreviewPlayer;

    HWND m_hAlbumViewerWnd;
    HWND m_hCreateAlbum;
    HWND m_hEditAnimation;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;

    void SetPreviewTexture(CTexture* _pTexture) { m_PreviewTexture = _pTexture; }
    void SetPreviewPlayer(CAlbumPlayer* _pAlbPlayer);
    CAlbumPlayer* GetPreviewPlayer() { return m_PreviewPlayer; }


    void LoadNPKCallback();
    void CreateAlbumCallback();
    void EditAnimationCallback();
};

