#include "pch.h"
#include "CStageMaker.h"
#include "CLevelMgr.h"
#include "CDungeonMaker.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CSound.h"
#include "CSoundMgr.h"
#include "CEngine.h"
#include "CTextMgr.h"
#include "CPortal.h"
#include "CMonster.h"
#include "CWall.h"
#include "CNPC.h"


CStageMaker::CStageMaker()
	: CLevel(L"StageMaker")
	, m_StageInfo(nullptr)
	, m_Background(nullptr)
	, m_BGM(nullptr)
{
}

CStageMaker::~CStageMaker()
{
}

void CStageMaker::Begin()
{
	// 던전메이커 레벨에서 선택된 타일의 스테이지 정보를 가져옴
	CDungeonMaker* pDM = dynamic_cast<CDungeonMaker*>(CLevelMgr::GetInst()->FindLevel(L"DungeonMaker"));
	assert(pDM);
	m_StageInfo = pDM->FindStageInfo(pDM->GetSelectedTile());

	// 배경 객체 생성 후 애니메이션 입력
	m_Background = new CBackground(L"StageMakerBackground");
	m_Background->SetScale(m_StageInfo->StageSize);
	for (int i = 0; i < m_StageInfo->vecBGA.size(); ++i)
	{
		m_Background->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"BGA_" + std::to_wstring(i)
			, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + m_StageInfo->vecBGA[i]));
	}

	// BGM이 있으면 재생
	if (!m_StageInfo->BGMPath.empty())
	{
		m_BGM = CSoundMgr::GetInst()->GetSound(L"StageMakerBGM"
			, CEngine::GetInst()->GetResourcePathW() + L"\\music\\" + m_StageInfo->BGMPath);
	}

	// 포탈 데이터 조정
	for (int dir = 0; dir < (int)PortalDirection::END; ++dir)
	{
		// 유효한 포탈이 위치가 지정된 적이 없는 경우(-1, -1), 디폴트 위치로 배치
		if (m_StageInfo->arrPortalInfo[dir].PointTile != Vec2D(-1, -1) && m_StageInfo->arrPortalInfo[dir].Location == Vec2D(-1, -1))
		{
			switch ((PortalDirection)dir)
			{
			case PortalDirection::TOP:
				m_StageInfo->arrPortalInfo[dir].Location = Vec2D(m_Background->GetScale().x / 2, 0.f);
				break;
			case PortalDirection::LEFT:
				m_StageInfo->arrPortalInfo[dir].Location = Vec2D(0.f, m_Background->GetScale().y / 2);
				break;
			case PortalDirection::RIGHT:
				m_StageInfo->arrPortalInfo[dir].Location = Vec2D(m_Background->GetScale().x, m_Background->GetScale().y / 2);
				break;
			case PortalDirection::BOTTOM:
				m_StageInfo->arrPortalInfo[dir].Location = Vec2D(m_Background->GetScale().x / 2, m_Background->GetScale().y);
				break;
			}
		}
	}

	// 포탈, 벽, 몬스터, NPC 오브젝트 추가
	// Tick을 무시하고 Render만 되어야 하므로 레벨에서 직접 관리
	for (int i = 0; i < (int)PortalDirection::END; ++i)
	{
		if (m_StageInfo->arrPortalInfo[i].PointTile != Vec2D(-1, -1))
		{
			CPortal* pPortal = new CPortal(L"Dir_" + std::to_wstring(i));
			pPortal->SetLocation(m_StageInfo->arrPortalInfo[i].Location);

		}
	}

}

void CStageMaker::Tick()
{

}

void CStageMaker::FinalTick()
{

}

void CStageMaker::Render()
{
	CTextMgr::GetInst()->Render(10, 10, m_StageInfo->StageName, Color(255, 0, 0, 0), true);
}

void CStageMaker::End()
{
	if (m_Background)
	{
		delete m_Background;
		m_Background = nullptr;
	}
}
