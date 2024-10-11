#pragma once
#include "CResource.h"

class CAlbum;

class CTexture :
    public CResource
{
    friend class CTextureMgr;
public:
    CTexture(wstring _Name, CAlbum* _Owner);
    ~CTexture();

private:
    CAlbum* m_Owner;

    Bitmap* m_Bitmap;
    HDC m_DC;
    Vec2D m_Size;
    Vec2D m_Offset;


    // Texture 가 속한 Album 관련 정보
    int Index;
    ColorBits Type;
    CompressMode CompressMode;
    int Length;     // Album 내 데이터 길이
    Vec2D m_CanvasSize;
    CTexture* Target;
    char* Data;

    TextureVersion Version;
    int FullLength;


public:
    HDC GetDC() { return m_DC; }
    void SetSize(float _x, float _y) { m_Size.x = _x; m_Size.y = _y; }
    void SteOffset(float _x, float _y) { m_Offset.x = _x; m_Offset.y = _y; }
    void SetOwner(CAlbum* _Album) { m_Owner = _Album; }

    virtual int Load() override;
    void Render(Vec2D _PlayerOffset = Vec2D(0, 0));

};

