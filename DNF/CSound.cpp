#include "pch.h"
#include "CSound.h"

#include "CSoundMgr.h"
#include "CEngine.h"

CSound::CSound(wstring _name)
	: CResource(_name, ResourceType::Sound)
{
}

CSound::~CSound()
{
}


int CSound::Load(const wstring& _strFilePath)
{
	if (nullptr == CSoundMgr::GetInst()->GetSoundDevice())
		assert(nullptr);

	wstring strContent = CEngine::GetInst()->GetResourcePathW();
	strContent += _strFilePath;

	wchar_t szExt[10] = { 0 };
	_wsplitpath_s(strContent.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 10);

	if (!wcscmp(szExt, L".wav"))
	{
		if (false == LoadWaveSound(strContent))
			assert(nullptr);
	}
	else
		assert(nullptr);

	return true;
}



bool CSound::LoadWaveSound(const wstring& _strPath)
{
	HMMIO	hFile; // File Handle

	wstring strFilePath = _strPath;

	// CreateFile
	hFile = mmioOpen((wchar_t*)strFilePath.c_str(), NULL, MMIO_READ);// wave 파일 열기

	if (nullptr == hFile)
	{
		MessageBox(NULL, L"사운드 로딩 실패", L"에러", MB_OK);
		return false;
	}

	// Chunk, wave 파일 구조 분석
	MMCKINFO	pParent;
	memset(&pParent, 0, sizeof(pParent));
	pParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmioDescend(hFile, &pParent, NULL, MMIO_FINDRIFF);

	MMCKINFO	pChild;
	memset(&pChild, 0, sizeof(pChild));
	pChild.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	WAVEFORMATEX	wft;
	memset(&wft, 0, sizeof(wft));
	mmioRead(hFile, (char*)&wft, sizeof(wft));

	mmioAscend(hFile, &pChild, 0);
	pChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	memset(&m_tBuffInfo, 0, sizeof(DSBUFFERDESC));
	m_tBuffInfo.dwBufferBytes = pChild.cksize;
	m_tBuffInfo.dwSize = sizeof(DSBUFFERDESC);
	m_tBuffInfo.dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLVOLUME;
	m_tBuffInfo.lpwfxFormat = &wft;

	if (FAILED(CSoundMgr::GetInst()->GetSoundDevice()->CreateSoundBuffer(&m_tBuffInfo, &m_pSoundBuffer, NULL)))
	{
		MessageBox(NULL, L"웨이브 파일 로딩 실패", L"에러", MB_OK);
		return false;
	}

	void* pWrite1 = NULL;
	void* pWrite2 = NULL;
	DWORD dwlength1, dwlength2;

	m_pSoundBuffer->Lock(0, pChild.cksize, &pWrite1, &dwlength1
		, &pWrite2, &dwlength2, 0L);

	if (pWrite1 != nullptr)
		mmioRead(hFile, (char*)pWrite1, dwlength1);
	if (pWrite2 != nullptr)
		mmioRead(hFile, (char*)pWrite2, dwlength2);

	m_pSoundBuffer->Unlock(pWrite1, dwlength1, pWrite2, dwlength2);

	mmioClose(hFile, 0);

	// 기본 볼륨 50으로 설정
	SetVolume(50.f);

	return true;
}

void CSound::Play(bool _bLoop)
{
	m_pSoundBuffer->SetCurrentPosition(0);

	if (_bLoop)
		m_pSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	else
		m_pSoundBuffer->Play(0, 0, 0);
}

void CSound::PlayToBGM(bool _bLoop)
{
	CSoundMgr::GetInst()->RegisterToBGM(this);

	if (_bLoop)
		m_pSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	else
		m_pSoundBuffer->Play(0, 0, 0);
}

void CSound::Stop(bool _bReset)
{
	m_pSoundBuffer->Stop();

	if (_bReset)
		m_pSoundBuffer->SetCurrentPosition(0);
}


void CSound::SetVolume(float _fVolume)
{
	m_iVolume = GetDecibel(_fVolume);
	m_pSoundBuffer->SetVolume(m_iVolume);
}

void CSound::SetPosition(float _fPosition)
{
	Stop(true);

	DWORD dwBytes = (DWORD)((_fPosition / 100.f) * (float)m_tBuffInfo.dwBufferBytes);
	m_pSoundBuffer->SetCurrentPosition(dwBytes);
}

int CSound::GetDecibel(float _fVolume)
{
	if (_fVolume > 100.f)
		_fVolume = 100.f;
	else if (_fVolume <= 0.f)
		_fVolume = 0.00001f;

	// 1 ~ 100 의 값을 데시벨로 변환
	int iVolume = (LONG)(-2000.0 * log10(100.f / _fVolume));

	if (iVolume < -10000)
		iVolume = -10000;
	return  iVolume;
}
