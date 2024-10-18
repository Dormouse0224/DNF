#pragma once

class CSound;

class CSoundMgr
{
	friend class CSound;
	SINGLETON(CSoundMgr)
private:
	map<wstring, CSound*> m_SoundMap;
	LPDIRECTSOUND8	m_SoundeDevice;		// 사운드 카드 제어
	CSound* m_pBGM;						// BGM Sound


public:
	int Init();
	LPDIRECTSOUND8 GetSoundDevice() { return m_SoundeDevice; }
	void RegisterToBGM(CSound* _pSound);
	CSound* GetSound(wstring _SoundName, const wstring& _strFilePath);
};

