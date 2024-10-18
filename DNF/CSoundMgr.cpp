#include "pch.h"
#include "CSoundMgr.h"

#include "CEngine.h"
#include "CSound.h"

CSoundMgr::CSoundMgr()
	: m_SoundeDevice(nullptr)
	, m_pBGM(nullptr)
{

}

CSoundMgr::~CSoundMgr()
{
	for (map<wstring, CSound*>::iterator iter = m_SoundMap.begin(); iter != m_SoundMap.end(); ++iter)
	{
		if (iter->second != nullptr)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}
	m_SoundMap.clear();
}

int CSoundMgr::Init()
{
	if (FAILED(DirectSoundCreate8(NULL, &m_SoundeDevice, NULL)))
	{
		MessageBox(NULL, L"사운드 디바이스 생성 실패", L"SYSTEM ERROR", MB_OK);
		return false;
	}

	// 사운드 협조레벨 설정
	HWND hWnd = CEngine::GetInst()->GetMainWnd();
	if (FAILED(m_SoundeDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE)))
	{
		MessageBox(NULL, L"사운드 매니저 초기화 실패", L"SYSTEM ERROR", MB_OK);
		return false;
	}

	return true;
}

void CSoundMgr::RegisterToBGM(CSound* _pSound)
{
	if (m_pBGM != nullptr)
		m_pBGM->Stop(true);

	m_pBGM = _pSound;
}

CSound* CSoundMgr::GetSound(wstring _SoundName, const wstring& _strFilePath)
{
	map<wstring, CSound*>::iterator iter = m_SoundMap.find(_SoundName);
	if (iter != m_SoundMap.end())
	{
		return iter->second;
	}
	else
	{
		CSound* pSound = new CSound(_SoundName, _strFilePath);
		return pSound;
	}
}
