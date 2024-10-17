#pragma once

class CSound;

class CSoundMgr
{
	SINGLETON(CSoundMgr)
private:
	LPDIRECTSOUND8		m_SoundeDevice;	// ���� ī�� ����
	CSound* m_pBGM;			// BGM Sound


public:
	int Init();
	LPDIRECTSOUND8 GetSoundDevice() { return m_SoundeDevice; }
	void RegisterToBGM(CSound* _pSound);
};

