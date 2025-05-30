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
    float m_angle;
    bool m_Dodge;

    int m_SceneNumber;
    float m_SceneTime;

    bool m_bLoadingEnd;
    float m_RenderPercent;
    float m_RenderPercentH;

public:
    void SetPlayInfo(int _Begin, int _End, bool _Loop, int _FPS, Vec2D m_Offset, float _angle, bool _Dodge);
    int GetFPS() { return m_FPS; }
    void SetFPS(int _FPS) { m_FPS = _FPS; }
    void SetAngle(float _angle) { m_angle = _angle; }
    void SetRenderPercent(float _percent) { m_RenderPercent = _percent; }
    void SetRenderPercentH(float _percent) { m_RenderPercentH = _percent; }
    Vec2D GetOffset() { return m_Offset; }
    void SetOffset(Vec2D&& _offset) { m_Offset = _offset; }
    void FinalTick() override;
    void Render(CObj* _thisObj, bool bCameraFallow = false, bool _bFlipHorizontal = false);
    void DirectRender(bool bCameraFallow = false);
    void NextScene();
    int GetCurSceneNum() { return m_SceneNumber; }
    int GetFinal() { return m_End - m_Begin; }
    void SetCurrentScene(int _SceneIndex);
    void ChangeAlbum(string _AlbumPath, wstring _NpkPath);
    static CAlbumPlayer* CreatePlayerFromFile(wstring _Name, wstring _filepath, Vec2D&& _AdditionalOffset = Vec2D(0, 0));
    static void CreateAvatar(wstring _fileName, string _code, AvatarParts _parts, vector<CAlbumPlayer*>& _avatarArr);
    static void CreateAvatar(wstring _fileName, string _code[6], vector<CAlbumPlayer*>& _avatarArr);
    static vector<CAlbumPlayer*> CreateAvatarArr(wstring _fileName, string _code[6]);
    void SwitchLoop() { m_Loop = !m_Loop; }


};

