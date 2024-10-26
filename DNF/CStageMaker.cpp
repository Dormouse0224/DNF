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
#include "CTextureMgr.h"
#include "CCollider.h"
#include "CKeyMgr.h"
#include "CSticker.h"
#include "CDbgRender.h"
#include "DialogProc.h"
#include "resource.h"

CStageMaker::CStageMaker()
	:CStage(L"StageMaker")
	, m_hControlPanel(nullptr)
	, m_hSelectedObj(nullptr)
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
	CBackground* pBackground = new CBackground(L"StageMakerBackground");
	pBackground->SetScale(m_StageInfo->StageSize);
	AddObject(pBackground, LayerType::Background);
	for (int i = 0; i < m_StageInfo->vecBGA.size(); ++i)
	{
		pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"BGA_" + std::to_wstring(i)
			, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + m_StageInfo->vecBGA[i]));
	}


	// ��Ż ������ ����
	for (int dir = 0; dir < (int)PortalDirection::END; ++dir)
	{
		// ��ȿ�� ��Ż�� ��ġ�� ������ ���� ���� ���(-1, -1), ����Ʈ ��ġ�� ��ġ
		if (m_StageInfo->arrPortalInfo[dir].PointStageName != L"" && m_StageInfo->arrPortalInfo[dir].Location == Vec2D(-1, -1))
		{
			switch ((PortalDirection)dir)
			{
			case PortalDirection::TOP:
				m_StageInfo->arrPortalInfo[dir].Location = Vec2D(pBackground->GetScale().x / 2, 0.f);
				break;
			case PortalDirection::LEFT:
				m_StageInfo->arrPortalInfo[dir].Location = Vec2D(0.f, pBackground->GetScale().y / 2);
				break;
			case PortalDirection::RIGHT:
				m_StageInfo->arrPortalInfo[dir].Location = Vec2D(pBackground->GetScale().x, pBackground->GetScale().y / 2);
				break;
			case PortalDirection::BOTTOM:
				m_StageInfo->arrPortalInfo[dir].Location = Vec2D(pBackground->GetScale().x / 2, pBackground->GetScale().y);
				break;
			}
		}
	}

	// ��Ż, ��, ����, NPC ������Ʈ �߰�
	for (int i = 0; i < (int)PortalDirection::END; ++i)
	{
		if (m_StageInfo->arrPortalInfo[i].PointStageName != L"")
		{
			// ��Ż ��ü ����
			CPortal* pPortal = new CPortal(L"Portal_Dir_" + std::to_wstring(i));
			pPortal->SetLocation(m_StageInfo->arrPortalInfo[i].Location);
			pPortal->SetDest(CLevelMgr::GetInst()->FindLevel(m_StageInfo->arrPortalInfo[i].PointStageName));
			pPortal->SetScale(Vec2D(60, 120));
			AddObject(pPortal, LayerType::Object);
			// �浹ü ���
			CCollider* pCollider = new CCollider(L"Portal_Col_" + std::to_wstring(i));
			pCollider->SetSize(pPortal->GetScale());
			pPortal->AddComponent(pCollider);
			// �ִϸ��̼� ���
			pPortal->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"Portal_AP_" + std::to_wstring(i)
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\gatedown_n.animation"));
			pPortal->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"Portal_AP_" + std::to_wstring(i)
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\gatedown_d.animation"));
			pPortal->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"Portal_AP_" + std::to_wstring(i)
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\gateup_n.animation"));
			pPortal->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"Portal_AP_" + std::to_wstring(i)
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\gateup_d.animation"));
			pPortal->AddDeactiveAnimation(CAlbumPlayer::CreatePlayerFromFile(L"Portal_AP_" + std::to_wstring(i)
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\gatedown_n.animation"));
			pPortal->AddDeactiveAnimation(CAlbumPlayer::CreatePlayerFromFile(L"Portal_AP_" + std::to_wstring(i)
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\gatedown_d_red.animation"));
			pPortal->AddDeactiveAnimation(CAlbumPlayer::CreatePlayerFromFile(L"Portal_AP_" + std::to_wstring(i)
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\gateup_n.animation"));
			pPortal->AddDeactiveAnimation(CAlbumPlayer::CreatePlayerFromFile(L"Portal_AP_" + std::to_wstring(i)
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\gateup_d_red.animation"));
			// ��ƼĿ ���
			pPortal->AddComponent(new CSticker(L"Portal_Stk_"));
		}
	}





	// BGM�� ������ ���
	if (!m_StageInfo->BGMPath.empty())
	{
		m_BGM = CSoundMgr::GetInst()->GetSound(L"StageMakerBGM", L"\\music\\" + m_StageInfo->BGMPath);
		m_BGM->SetVolume(50);
		m_BGM->PlayToBGM(true);
	}

	m_hControlPanel = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_StageMakerPanel), CEngine::GetInst()->GetMainWnd(), &StageMakerPanelProc);
	ShowWindow(m_hControlPanel, SW_SHOWNORMAL);
	m_hSelectedObj = GetDlgItem(m_hControlPanel, STATIC_SelectedObj);
}

void CStageMaker::Tick()
{
	CStage::Tick();
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::ESC) == Key_state::TAP)
	{
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"DungeonMaker"));
	}

	if (GetSelectedObj() != nullptr && m_hSelectedObj != nullptr)
		SetWindowText(m_hSelectedObj, (L"ID: " + std::to_wstring(GetSelectedObj()->GetID()) + L", Name: " + GetSelectedObj()->GetName()).c_str());
	else
		SetWindowText(m_hSelectedObj, L"(NONE)");
}

void CStageMaker::FinalTick()
{
	CStage::FinalTick();
}

void CStageMaker::Render()
{
	CStage::Render();

	CTextMgr::GetInst()->Render(10, 10, m_StageInfo->StageName, Color(255, 0, 0, 0), true);

}

void CStageMaker::End()
{
	DestroyWindow(m_hControlPanel);
	m_hControlPanel = nullptr;
	m_hSelectedObj = nullptr;
	CStage::End();
}
