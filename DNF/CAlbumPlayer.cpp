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
	m_CurrentAlbum->GetScene(m_SceneNumber + m_Begin)->Render(m_Offset, m_angle, bCameraFallow, m_Dodge, _bFlipHorizontal);
}

// 오브젝트를 거치지 않고 직접 렌더링
void CAlbumPlayer::DirectRender(bool bCameraFallow)
{
	m_bLoadingEnd = true;
	m_CurrentAlbum->GetScene(m_SceneNumber + m_Begin)->DirectRender(m_Offset, m_angle, bCameraFallow, m_Dodge);
}

void CAlbumPlayer::NextScene()
{
	if (m_Loop)
	{
		if ((m_SceneNumber + m_Begin) == m_End)
		{
			m_SceneNumber = 0;
		}
		else
		{
			++m_SceneNumber;
		}
	}
}

void CAlbumPlayer::SetCurrentScene(int _SceneIndex)
{
	assert(_SceneIndex <= m_End - m_Begin && _SceneIndex >= 0);
	m_SceneNumber = _SceneIndex;
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
