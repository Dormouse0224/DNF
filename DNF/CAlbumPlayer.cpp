#include "pch.h"
#include "CAlbumPlayer.h"
#include "CTimeMgr.h"
#include "CAlbum.h"
#include "CTexture.h"
#include "CTextureMgr.h"

CAlbumPlayer::CAlbumPlayer(wstring _Name, string _AlbumPath, wstring _NpkPath)
	: CComponent(_Name)
	, m_Begin(0)
	, m_End(0)
	, m_Loop(false)
	, m_CurrentAlbum(nullptr)
	, m_FPS(0)
	, m_SceneNumber(0)
	, m_SceneTime(0)
{
	m_CurrentAlbum = CTextureMgr::GetInst()->LoadAlbum(_AlbumPath, _NpkPath);
}

CAlbumPlayer::~CAlbumPlayer()
{

}

void CAlbumPlayer::SetPlayInfo(int _Begin, int _End, bool _Loop, int _FPS, Vec2D _Offset)
{
	assert(!(_End >= m_CurrentAlbum->m_SceneVector.size() || _Begin >= m_CurrentAlbum->m_SceneVector.size() || _Begin < 0 || _End < 0));
	m_SceneNumber = 0;
	m_SceneTime = 0;
	m_Begin = _Begin;
	m_End = _End;
	m_Loop = _Loop;
	m_FPS = _FPS;
	m_Offset = _Offset;
}

void CAlbumPlayer::FinalTick()
{
	if (m_FPS == 0)
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

void CAlbumPlayer::Render(CObj* _thisObj)
{
	// 현재 씬을 재생
	m_CurrentAlbum->m_Owner = _thisObj;
	m_CurrentAlbum->GetScene(m_SceneNumber + m_Begin)->Render(m_Offset);
}
