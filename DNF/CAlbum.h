#pragma once
#include "CResource.h"

class CTexture;

class CAlbum :
    public CResource
{
    friend class CNpkMgr;
    friend class CTextureMgr;
    friend class CAlbumPlayer;
public:
    CAlbum(wstring _NpkPath);
    ~CAlbum();

private:
    CObj*                   m_Owner;

    vector<CTexture*>       m_SceneVector;

    // NPK 파일 내 Album 의 정보
    int                     Offset;         // 파일 내 데이터 위치 오프셋
    int                     Length;         // 파일 내 데이터의 길이
    string                  Path;           // 경로

    LONGLONG                IndexLength;
    ImgVersion              Version;
    int                     Count;

    wstring                 m_OwnerNpk;

    vector<vector<Color>>   m_PaletteVector;

    vector<CTexture*>       AtlasVec;
    map<int, TextureInfo>   m_Map;


public:
    //void SetSceneCount(int _Count) { m_SceneCount = _Count; }
    virtual int Load() override;
    CTexture* GetScene(int _SceneIndex) { return m_SceneVector[_SceneIndex]; }
    CTexture* GetScene(wstring _name);
    LONGLONG GetIndexLength() { return IndexLength; }
    int GetCount() { return Count; }
    int GetSceneCount() { return (int)m_SceneVector.size(); }
    void AddScene(CTexture* _scene) { m_SceneVector.push_back(_scene); }
    CObj* GetOwner() { return m_Owner; }
    string GetPath() { return Path; }
    ImgVersion GetVersion() { return Version; }
    vector<vector<Color>> GetPaletteVector() { return m_PaletteVector; }
    void SetPaletteVector(vector<vector<Color>> _paletteVector) { m_PaletteVector = _paletteVector; }


};

