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

    // GDI+
    //GdiplusStartupInput GdiplusStartupInput;
    //ULONG_PTR GdiplusToken;

    // Texture �� ���� Album ���� ����
    int Index;
    ColorBits Type;
    CompressMode CompressMode;
    int Length;     // Album �� ������ ����
    Vec2D m_CanvasSize;
    CTexture* Target;
    char* Data;

    TextureVersion Version;
    int FullLength;


public:
    HDC GetDC() { return m_DC; }
    void SetSize(float _x, float _y) { m_Size.x = _x; m_Size.y = _y; }
    void SteOffset(float _x, float _y) { m_Offset.x = _x; m_Offset.y = _y; }

    virtual int Load() override;
    void Render();

};

