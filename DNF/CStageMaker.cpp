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
	// ��������Ŀ �������� ���õ� Ÿ���� �������� ������ ������
	CDungeonMaker* pDM = dynamic_cast<CDungeonMaker*>(CLevelMgr::GetInst()->FindLevel(L"DungeonMaker"));
	assert(pDM);
	m_StageInfo = pDM->FindStageInfo(pDM->GetSelectedTile());

	// ��� ��ü ���� �� �ִϸ��̼� �Է�
	m_Background = new CBackground(L"StageMakerBackground");
	m_Background->SetScale(m_StageInfo->StageSize);
	for (int i = 0; i < m_StageInfo->vecBGA.size(); ++i)
	{
		m_Background->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"BGA_" + std::to_wstring(i)
			, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + m_StageInfo->vecBGA[i]));
	}

	// BGM�� ������ ���
	if (!m_StageInfo->BGMPath.empty())
	{
		m_BGM = CSoundMgr::GetInst()->GetSound(L"StageMakerBGM"
			, CEngine::GetInst()->GetResourcePathW() + L"\\music\\" + m_StageInfo->BGMPath);
	}

	// ��Ż ������ ����
	for (int dir = 0; dir < (int)PortalDirection::END; ++dir)
	{
		// ��ȿ�� ��Ż�� ��ġ�� ������ ���� ���� ���(-1, -1), ����Ʈ ��ġ�� ��ġ
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

	// ��Ż, ��, ����, NPC ������Ʈ �߰�
	// Tick�� �����ϰ� Render�� �Ǿ�� �ϹǷ� �������� ���� ����
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
