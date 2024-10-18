#pragma once
#include "CResource.h"

class CSound :
    public CResource
{
public:
    CSound(wstring _name, const wstring& _strFilePath);
    ~CSound();

private:
    wstring                 m_FilePath;
    LPDIRECTSOUNDBUFFER		m_pSoundBuffer;
    DSBUFFERDESC			m_tBuffInfo;
    int						m_iVolume;


public:
    virtual int Load();

private:
    bool LoadWaveSound(const wstring& _strPath);
    bool LoadOGGSound(const wstring& _strPath);

public:
    // 사운드 재생
    void Play(bool _bLoop = false);

    // 배경음악으로 재생
    void PlayToBGM(bool _bLoop = false);

    // 정지
    void Stop(bool _bReset = false);

    // 볼륨 설정 (0 ~ 100)
    void SetVolume(float _fVolume);

    // 사운드 재생 위치 (0 ~ 100)
    void SetPosition(float _fPosition); // 0 ~ 100 

    int GetDecibel(float _fVolume);


};

