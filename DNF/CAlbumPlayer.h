#pragma once
#include "CComponent.h"


class CAlbum;


class CAlbumPlayer :
    public CComponent
{
public:
    CAlbumPlayer(wstring _Name, string _AlbumPath, wstring _NpkPath);
    ~CAlbumPlayer();

private:
    CAlbum* m_CurrentAlbum;
    int m_FPS;
    int m_Begin;
    int m_End;
    bool m_Loop;

    int m_SceneNumber;
    float m_SceneTime;

public:
    void SetPlayInfo(int _Begin, int _End, bool _Loop, int _FPS);
    void SetFPS(int _FPS) { m_FPS = _FPS; }
    void FinalTick() override;
    void Render(CObj* _thisObj);

};

