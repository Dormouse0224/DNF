#include "pch.h"
#include "CAlbumPlayer.h"
#include "CTimeMgr.h"
#include "CAlbum.h"
#include "CTexture.h"
#include "CTextureMgr.h"
#include "CEngine.h"

CAlbumPlayer::CAlbumPlayer(wstring _Name, string _AlbumPath, wstring _NpkPath)
	: CComponent(_Name)
	, m_Begin(0)
	, m_End(0)
	, m_Loop(false)
	, m_CurrentAlbum(nullptr)
	, m_FPS(0)
	, m_SceneNumber(0)
	, m_SceneTime(0)
	, m_Dodge(false)
	, m_bLoadingEnd(false)
	, m_RenderPercent(1.f)
	, m_RenderPercentH(1.f)
{
	m_CurrentAlbum = CTextureMgr::GetInst()->LoadAlbum(_AlbumPath, _NpkPath);
}

CAlbumPlayer::~CAlbumPlayer()
{

}

void CAlbumPlayer::SetPlayInfo(int _Begin, int _End, bool _Loop, int _FPS, Vec2D _Offset, float _angle, bool _Dodge)
{
	assert(!(_End >= m_CurrentAlbum->m_SceneVector.size() || _Begin >= m_CurrentAlbum->m_SceneVector.size() || _Begin < 0 || _End < 0));
	m_SceneNumber = 0;
	m_SceneTime = 0;
	m_Begin = _Begin;
	m_End = _End;
	m_Loop = _Loop;
	m_FPS = _FPS;
	m_Offset = _Offset;
	m_angle = _angle;
	m_Dodge = _Dodge;
}

void CAlbumPlayer::FinalTick()
{
	if (m_FPS == 0)
		return;

	if (!m_bLoadingEnd)
		return;

	if (m_SceneTime > (1 / (float)m_FPS))
	{
		if ((m_SceneNumber + m_Begin) == m_End)
		{
			if (m_Loop)
			{
				m_SceneNumber = 0;
				m_SceneTime -= (1 / (float)m_FPS);
			}
		}
		else
		{
			++m_SceneNumber;
			m_SceneTime -= (1 / (float)m_FPS);
		}
	}
	m_SceneTime += CTimeMgr::GetInst()->GetDeltaTime();

}

void CAlbumPlayer::Render(CObj* _thisObj, bool bCameraFallow, bool _bFlipHorizontal)
{
	m_bLoadingEnd = true;
	// 현재 씬을 재생
	m_CurrentAlbum->m_Owner = _thisObj;
	m_CurrentAlbum->GetScene(m_SceneNumber + m_Begin)->Render(m_Offset, m_angle, bCameraFallow, m_Dodge, _bFlipHorizontal, m_RenderPercent, m_RenderPercentH);
}

// 오브젝트를 거치지 않고 직접 렌더링
void CAlbumPlayer::DirectRender(bool bCameraFallow)
{
	m_bLoadingEnd = true;
	m_CurrentAlbum->GetScene(m_SceneNumber + m_Begin)->DirectRender(m_Offset, m_angle, bCameraFallow, m_Dodge);
}

void CAlbumPlayer::NextScene()
{
	if ((m_SceneNumber + m_Begin) == m_End)
	{
		if (m_Loop)
			m_SceneNumber = 0;
		else
			return;
	}
	else
		++m_SceneNumber;

	m_SceneTime = 0.f;
}

void CAlbumPlayer::SetCurrentScene(int _SceneIndex)
{
	assert(_SceneIndex <= m_End - m_Begin && _SceneIndex >= 0);
	m_SceneNumber = _SceneIndex;
	m_SceneTime = 0.f;
}

void CAlbumPlayer::ChangeAlbum(string _AlbumPath, wstring _NpkPath)
{
	m_CurrentAlbum = CTextureMgr::GetInst()->LoadAlbum(_AlbumPath, _NpkPath);
}

CAlbumPlayer* CAlbumPlayer::CreatePlayerFromFile(wstring _Name, wstring _filepath, Vec2D&& _AdditionalOffset)
{
	ifstream animation;
	AnimationInfo desc;
	animation.open(_filepath.c_str(), ios::binary);
	if (animation.is_open())
	{
		char NPKDir[255] = {};
		char AlbumPath[255] = {};
		animation.read((char*)&desc, sizeof(desc));
		animation.read(NPKDir, desc.NPKDirLen);
		animation.read(AlbumPath, desc.AlbumPathLen);

		animation.close();


		string strAlbumPath = AlbumPath;
		WCHAR wNPKDir[255] = {};
		MultiByteToWideChar(CP_ACP, 0, NPKDir, -1, wNPKDir, 255);
		wstring wstrNPKDir = wNPKDir;
		wstrNPKDir = CEngine::GetInst()->GetResourcePathW() + wstrNPKDir;


		CAlbumPlayer* pNewPlayer = new CAlbumPlayer(_Name, strAlbumPath, wstrNPKDir);
		pNewPlayer->SetPlayInfo(desc.IndexBegin, desc.IndexEnd, desc.bLoop, desc.FPS, desc.Offset + _AdditionalOffset, desc.angle, desc.bDodge);
		return pNewPlayer;
	}
	else
	{

		animation.close();
		return nullptr;
	}
}

void CAlbumPlayer::CreateAvatar(wstring _fileName, string _code, AvatarParts _parts, vector<CAlbumPlayer*>& _avatarArr)
{
	wstring _filepath = CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + _fileName + L".animation";
	ifstream animation;
	AnimationInfo desc;
	animation.open(_filepath.c_str(), ios::binary);
	if (animation.is_open())
	{
		char NPKDir[255] = {};
		char AlbumPath[255] = {};
		animation.read((char*)&desc, sizeof(desc));
		animation.read(NPKDir, desc.NPKDirLen);
		animation.read(AlbumPath, desc.AlbumPathLen);

		animation.close();

		string strAlbumPath = AlbumPath;
		WCHAR wNPKDir[255] = {};
		MultiByteToWideChar(CP_ACP, 0, NPKDir, -1, wNPKDir, 255);
		wstring wstrNPKDir = wNPKDir;
		wstrNPKDir = CEngine::GetInst()->GetResourcePathW() + wstrNPKDir;

		string parts[6] = { "hair", "cap", "pants", "coat", "shoes", "lbow" };
		wstring wparts[6] = { L"hair", L"cap", L"pants", L"coat", L"shoes", L"lbow" };
		string layer[4] = { "d", "c", "b", "a" };

		// 아바타가 속한 NPK 파일명
		int pos0 = wstrNPKDir.find(L"skin");
		if (pos0 != wstring::npos)
			wstrNPKDir.replace(pos0, 4, wparts[(int)_parts]);
		if (_parts == AvatarParts::LBow)
		{
			int pos = wstrNPKDir.find(L"avatar");
			if (pos != wstring::npos)
				wstrNPKDir.replace(pos, 6, L"weapon");
		}

		// Coat 레이어에만 존재하는 x 레이어는 따로 처리
		if (_parts == AvatarParts::Coat)
		{
			string temp = strAlbumPath;
			int pos1 = temp.find("skin");
			if (pos1 != string::npos)
				temp.replace(pos1, 4, parts[(int)_parts]);
			int pos2 = temp.find("body");
			if (pos2 != string::npos)
				temp.replace(pos2, 8, parts[(int)_parts] + _code + "x");

			if (CTextureMgr::GetInst()->LoadAlbum(temp, wstrNPKDir))
			{
				delete _avatarArr[0];
				_avatarArr[0] = new CAlbumPlayer(_fileName, temp, wstrNPKDir);
				_avatarArr[0]->SetPlayInfo(desc.IndexBegin, desc.IndexEnd, desc.bLoop, desc.FPS, desc.Offset, desc.angle, desc.bDodge);
			}
		}

		// 아바타가 속한 앨범명 계산 후 배열에 저장
		for (int layerIdx = 0; layerIdx < 4; ++layerIdx)
		{
			string temp = strAlbumPath;
			int pos1 = temp.find("skin");
			if (pos1 != string::npos)
				temp.replace(pos1, 4, parts[(int)_parts]);
			if (_parts == AvatarParts::LBow)
			{
				int pos = temp.find("avatar");
				if (pos != string::npos)
					temp.replace(pos, 6, "weapon");
			}
			int pos2 = temp.find("body");
			if (pos2 != string::npos)
				temp.replace(pos2, 8, parts[(int)_parts] + _code + layer[layerIdx]);

			if (CTextureMgr::GetInst()->LoadAlbum(temp, wstrNPKDir))
			{
				delete _avatarArr[1 + (int)_parts + (layerIdx * 6)];
				_avatarArr[1 + (int)_parts + (layerIdx * 6)] = new CAlbumPlayer(_fileName, temp, wstrNPKDir);
				_avatarArr[1 + (int)_parts + (layerIdx * 6)]->SetPlayInfo(desc.IndexBegin, desc.IndexEnd, desc.bLoop, desc.FPS, desc.Offset, desc.angle, desc.bDodge);
			}
		}
	}
	else
	{
		animation.close();
	}
}


void CAlbumPlayer::CreateAvatar(wstring _fileName, string _code[6], vector<CAlbumPlayer*>& _avatarArr)
{
	for (int i = 0; i < (int)AvatarParts::END; ++i)
	{
		CreateAvatar(_fileName, _code[i], (AvatarParts)i, _avatarArr);
	}
}

vector<CAlbumPlayer*> CAlbumPlayer::CreateAvatarArr(wstring _fileName, string _code[6])
{
	vector<CAlbumPlayer*> _avatarArr(25, nullptr);
	for (int i = 0; i < (int)AvatarParts::END; ++i)
	{
		CreateAvatar(_fileName, _code[i], (AvatarParts)i, _avatarArr);
	}
	return _avatarArr;
}