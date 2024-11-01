#include "pch.h"
#include "CSound.h"

#include "CSoundMgr.h"
#include "CEngine.h"

CSound::CSound(wstring _name, const wstring& _strFilePath)
	: CResource(_name, ResourceType::Sound)
	, m_FilePath(_strFilePath)
{
	Load();
}

CSound::~CSound()
{
}


int CSound::Load()
{
	assert(CSoundMgr::GetInst()->m_SoundMap.insert(make_pair(GetName(), this)).second);

	if (nullptr == CSoundMgr::GetInst()->GetSoundDevice())
		assert(nullptr);

	wstring strContent = CEngine::GetInst()->GetResourcePathW();
	strContent += m_FilePath;

	wchar_t szExt[10] = { 0 };
	_wsplitpath_s(strContent.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 10);

	if (!wcscmp(szExt, L".wav"))
	{
		if (false == LoadWaveSound(strContent))
			assert(nullptr);
	}
	else if (!wcscmp(szExt, L".ogg"))
	{
		if (false == LoadOGGSound(strContent))
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
	hFile = mmioOpen((wchar_t*)strFilePath.c_str(), NULL, MMIO_READ);// wave ���� ����

	if (nullptr == hFile)
	{
		MessageBox(NULL, L"���� �ε� ����", L"����", MB_OK);
		return false;
	}

	// Chunk, wave ���� ���� �м�
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
		MessageBox(NULL, L"���̺� ���� �ε� ����", L"����", MB_OK);
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

	// �⺻ ���� 50���� ����
	SetVolume(50.f);

	return true;
}

bool CSound::LoadOGGSound(const wstring& _strPath)
{
	// ogg ������ �޸� ���ۿ� ����
	// �޸� ���۷κ��� ogg ������ �ε�
	// ogg �����ͷκ��� vorbis_info �� ��� waveformat �ۼ�
	// ogg �����͸� PCM �����ͷ� ��ȯ
	// 
	//

	// ���� ���� ���� �� ���� ������ ����
	ifstream oggfile(_strPath, ios::binary);
	assert(oggfile.is_open());
	oggfile.seekg(0, ios::end);
	int filesize = (int)oggfile.tellg();
	oggfile.seekg(0, ios::beg);
	vector<char> buffer(filesize);
	oggfile.read(buffer.data(), filesize);
	oggfile.close();

	ov_callbacks callbacks;
	OggVorbis_File oggFile;
	WAVEFORMATEX waveFormat;

	// �����Լ��� �ݹ� ����
	callbacks.read_func = [](void* ptr, size_t size, size_t nmemb, void* datasource) -> size_t
	{
		pair<const char*, size_t>& memoryBuffer = *(pair<const char*, size_t>*)(datasource);
		size_t bytesToRead = size * nmemb;
		if (bytesToRead > memoryBuffer.second)
		{
			bytesToRead = memoryBuffer.second;
		}
		memcpy(ptr, memoryBuffer.first, bytesToRead);
		memoryBuffer.first += bytesToRead;
		memoryBuffer.second -= bytesToRead;
		return bytesToRead;
	};

	callbacks.seek_func = [](void* datasource, ogg_int64_t offset, int whence) -> int 
	{
		return -1;
	};

	callbacks.close_func = [](void* datasource) -> int 
	{
		return 0;
	};

	callbacks.tell_func = [](void* datasource) -> long 
	{
		return 0;
	};


	// �޸� ���۷κ��� ogg ������ �ε�
	std::pair<const char*, size_t> memoryBuffer(buffer.data(), buffer.size());
	int res = ov_open_callbacks(&memoryBuffer, &oggFile, nullptr, 0, callbacks);
	assert(res >= 0);


	// Vorbis_info �� waveformat �ۼ�
	vorbis_info* info = ov_info(&oggFile, -1);
	int channels = info->channels;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = channels;
	waveFormat.nSamplesPerSec = info->rate;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nBlockAlign = (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// PCM �����͸� �޸𸮿� ����
	std::vector<char> pcmData;
	char pcmOut[4096];
	int bitstream;
	long bytesRead;
	while ((bytesRead = ov_read(&oggFile, pcmOut, sizeof(pcmOut), 0, 2, 1, &bitstream)) > 0)
	{
		pcmData.insert(pcmData.end(), pcmOut, pcmOut + bytesRead);
	}

	// ���ҽ� ����
	ov_clear(&oggFile);

	m_tBuffInfo.dwSize = sizeof(DSBUFFERDESC);
	m_tBuffInfo.dwFlags = DSBCAPS_CTRLVOLUME;
	m_tBuffInfo.dwBufferBytes = (DWORD)pcmData.size();
	m_tBuffInfo.lpwfxFormat = &waveFormat;

	HRESULT hr = CSoundMgr::GetInst()->GetSoundDevice()->CreateSoundBuffer(&m_tBuffInfo, &m_pSoundBuffer, NULL);
	if (FAILED(hr)) return false;

	// ���� ���
	void* pAudioPtr1 = nullptr;
	void* pAudioPtr2 = nullptr;
	DWORD audioBytes1 = 0;
	DWORD audioBytes2 = 0;
	hr = m_pSoundBuffer->Lock(0, (DWORD)pcmData.size(), &pAudioPtr1, &audioBytes1, &pAudioPtr2, &audioBytes2, 0);
	if (FAILED(hr)) return false;

	// PCM �����͸� ���ۿ� ����
	memcpy(pAudioPtr1, pcmData.data(), audioBytes1);
	if (pAudioPtr2)
	{
		memcpy(pAudioPtr2, pcmData.data() + audioBytes1, audioBytes2);
	}

	m_pSoundBuffer->Unlock(pAudioPtr1, audioBytes1, pAudioPtr2, audioBytes2);

	// �⺻ ���� 50���� ����
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

	// 1 ~ 100 �� ���� ���ú��� ��ȯ
	int iVolume = (LONG)(-2000.0 * log10(100.f / _fVolume));

	if (iVolume < -10000)
		iVolume = -10000;
	return  iVolume;
}
