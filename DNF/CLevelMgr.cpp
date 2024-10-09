#include "pch.h"
#include "CLevelMgr.h"
#include "CDbgRender.h"
#include "CTexture.h"
#include "CBackground.h"
#include "CEngine.h"
#include "CNpkMgr.h"

#include "CLevel_Start.h"
#include "CLevel_Edit.h"

#include "CAlbum.h"
#include "CAlbumPlayer.h"

CLevelMgr::CLevelMgr()
	: m_CurrentLevel(nullptr)
	, m_CLevels()
{

}

CLevelMgr::~CLevelMgr()
{
	for (map<wstring, CLevel*>::iterator iter = m_CLevels.begin(); iter != m_CLevels.end(); ++iter)
	{
		if (iter->second != nullptr)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}
}

void CLevelMgr::Init()
{
	// �ʱ� ���� ���� �� �߰�

	CLevel* pStartLevel = new CLevel_Start;
	m_CurrentLevel = pStartLevel;
	m_CurrentLevel->Begin();
}

CLevel* CLevelMgr::FindLevel(wstring _LevelName)
{
	map<wstring, CLevel*>::iterator iter = m_CLevels.find(_LevelName);
	if (iter == m_CLevels.end())
		return nullptr;
	else
		return iter->second;
}

void CLevelMgr::Progress()
{
	if (nullptr == m_CurrentLevel)
		return;

	m_CurrentLevel->Tick();
	m_CurrentLevel->FinalTick();
}

void CLevelMgr::Render()
{
	m_CurrentLevel->Render();

	if (CDbgRender::GetInst()->GetShowState())
	{
		CDbgRender::GetInst()->Render();
	}
}