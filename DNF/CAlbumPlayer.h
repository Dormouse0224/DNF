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
    Vec2D m_Offset;

    int m_SceneNumber;
    float m_SceneTime;

public:
    void SetPlayInfo(int _Begin, int _End, bool _Loop, int _FPS, Vec2D m_Offset);
    void SetFPS(int _FPS) { m_FPS = _FPS; }
    void FinalTick() override;
    void Render(CObj* _thisObj, bool bCameraDependent = true);
    void NextScene();
    void SetCurrentScene(int _SceneIndex);
    void ChangeAlbum(string _AlbumPath, wstring _NpkPath);
    static CAlbumPlayer* CreatePlayerFromFile(wstring _Name, string _filepath);
};

