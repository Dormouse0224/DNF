#include "pch.h"
#include "CCameraMgr.h"
#include "CTimeMgr.h"
#include "CEngine.h"
#include "CLevelMgr.h"
#include "CKeyMgr.h"
#include "CTextureMgr.h"
#include "CStage.h"
#include "CPlayer.h"


CCameraMgr::CCameraMgr()
	: m_CameraPos(Vec2D(0, 0))
	, m_CameraSpeed(500.f)
	, m_Alpha(0)
{

}

CCameraMgr::~CCameraMgr()
{

}

void CCameraMgr::Init()
{

}

void CCameraMgr::Tick()
{
	wstring levelName = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();
	if (levelName == L"Level_Edit" || levelName == L"DungeonMaker" || levelName == L"StageMaker")
	{
		Key_state W = CKeyMgr::GetInst()->GetKeyState(Keyboard::W);
		Key_state A = CKeyMgr::GetInst()->GetKeyState(Keyboard::A);
		Key_state S = CKeyMgr::GetInst()->GetKeyState(Keyboard::S);
		Key_state D = CKeyMgr::GetInst()->GetKeyState(Keyboard::D);
		Key_state Space = CKeyMgr::GetInst()->GetKeyState(Keyboard::SPACE);
		if (W == Key_state::PRESSED || W == Key_state::TAP)
		{
			m_CameraPos.y -= m_CameraSpeed * CTimeMgr::GetInst()->GetDeltaTime();
		}
		if (A == Key_state::PRESSED || A == Key_state::TAP)
		{
			m_CameraPos.x -= m_CameraSpeed * CTimeMgr::GetInst()->GetDeltaTime();
		}
		if (S == Key_state::PRESSED || S == Key_state::TAP)
		{
			m_CameraPos.y += m_CameraSpeed * CTimeMgr::GetInst()->GetDeltaTime();
		}
		if (D == Key_state::PRESSED || D == Key_state::TAP)
		{
			m_CameraPos.x += m_CameraSpeed * CTimeMgr::GetInst()->GetDeltaTime();
		}
		if (Space == Key_state::PRESSED || Space == Key_state::TAP)
		{
			m_CameraPos = Vec2D(0, 0);
		}
	}
	else
	{
		Vec2D res = CEngine::GetInst()->GetResolution();
		CStage* pStage = dynamic_cast<CStage*>(CLevelMgr::GetInst()->GetCurrentLevel());
		if (pStage)
		{
			m_CameraPos = pStage->GetPlayer()->GetGroundPos() - CEngine::GetInst()->GetResolution() / 2.f;
			m_CameraPos.x = max(0.0f, min(m_CameraPos.x, pStage->GetStageInfo()->StageSize.x - res.x));
			m_CameraPos.y = max(0.0f, min(m_CameraPos.y, pStage->GetStageInfo()->StageSize.y - res.y));

		}
	}

}


