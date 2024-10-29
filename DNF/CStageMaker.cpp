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

#include <Commctrl.h>

CStageMaker::CStageMaker()
	:CStage(L"StageMaker")
	, m_hControlPanel(nullptr)
	, m_hSelectedObj(nullptr)
	, m_hUpperBoundSlide(nullptr)
	, m_hUpperBoundStatic(nullptr)
	, m_PortalArr{}
{
}

CStageMaker::~CStageMaker()
{
	m_StageInfo = nullptr;
}

void CStageMaker::Begin()
{
	// 던전메이커 레벨에서 선택된 타일의 스테이지 정보를 가져옴
	CDungeonMaker* pDM = dynamic_cast<CDungeonMaker*>(CLevelMgr::GetInst()->FindLevel(L"DungeonMaker"));
	assert(pDM);
	m_StageInfo = pDM->FindStageInfo(pDM->GetSelectedTile());

	// 배경 객체 생성 후 애니메이션 입력
	CBackground* pBackground = new CBackground(L"StageMakerBackground");
	pBackground->SetScale(m_StageInfo->StageSize);
	AddObject(pBackground, LayerType::Background);
	for (int i = 0; i < m_StageInfo->vecBGA.size(); ++i)
	{
		pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"BGA_" + std::to_wstring(i)
			, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + m_StageInfo->vecBGA[i]));
	}

	// 플레이어 영역 upperbound 세팅
	m_UpperBound = m_StageInfo->UpperBound;

	// 포탈 데이터 조정
	for (int dir = 0; dir < (int)PortalDirection::END; ++dir)
	{
		// 유효한 포탈이 위치가 지정된 적이 없는 경우(-1, -1), 디폴트 위치로 배치
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

	// 포탈 오브젝트 추가
	for (int i = 0; i < (int)PortalDirection::END; ++i)
	{
		if (m_StageInfo->arrPortalInfo[i].PointStageName != L"")
		{
			// 포탈 객체 생성
			CPortal* pPortal = new CPortal(L"Portal_Dir_" + std::to_wstring(i));
			pPortal->SetLocation(m_StageInfo->arrPortalInfo[i].Location);
			pPortal->SetDest(CLevelMgr::GetInst()->FindLevel(m_StageInfo->arrPortalInfo[i].PointStageName));
			pPortal->SetScale(Vec2D(60, 150));
			AddObject(pPortal, LayerType::Object);
			// 충돌체 등록
			CCollider* pCollider = new CCollider(L"Portal_Col_" + std::to_wstring(i));
			pCollider->SetSize(pPortal->GetScale());
			pPortal->AddComponent(pCollider);
			pPortal->RegisterBodyCollider(pCollider);
			// 애니메이션 등록
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
			// 스티커 등록
			pPortal->AddComponent(new CSticker(L"Portal_Stk_"));

			m_PortalArr[i] = pPortal;
		}
	}

	// 몬스터 오브젝트 추가
	for (MonsterInfo* pMonsterInfo : m_StageInfo->vecMonsterInfo)
	{
		switch (pMonsterInfo->MonsterTemplate)
		{
		case MonsterTemplate::bloatedfat:
		{

			break;
		}
		case MonsterTemplate::dominatedunnaturals:
		{
			CMonster* pMonster = new CMonster(pMonsterInfo->Name);
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pMonster, LayerType::Object);
			pMonster->AddComponent(new CSticker(L"dominatedunnaturals_Stk"));
			pMonster->SetLocation(pMonsterInfo->pos);
			pMonster->SetScale(Vec2D(100, 140));
			CCollider* pMonCollider = new CCollider(pMonsterInfo->Name + L"_Col");
			pMonCollider->SetSize(pMonster->GetScale());
			pMonster->AddComponent(pMonCollider);
			pMonster->RegisterBodyCollider(pMonCollider);
			pMonster->SetMonsterTemplate(MonsterTemplate::dominatedunnaturals);
			pMonster->AddAnimation(MonsterState::Idle, CAlbumPlayer::CreatePlayerFromFile(L"dominatedunnaturals_Idle"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\monster_dominatedunnaturals_Idle.animation"));
			pMonster->AddAnimation(MonsterState::Move, CAlbumPlayer::CreatePlayerFromFile(L"dominatedunnaturals_Move"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\monster_dominatedunnaturals_Move.animation"));
			pMonster->AddAnimation(MonsterState::Attack, CAlbumPlayer::CreatePlayerFromFile(L"dominatedunnaturals_Attack0"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\monster_dominatedunnaturals_Attack0.animation"));
			pMonster->AddAnimation(MonsterState::Attack, CAlbumPlayer::CreatePlayerFromFile(L"dominatedunnaturals_Attack1"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\monster_dominatedunnaturals_Attack1.animation"));
			pMonster->AddAnimation(MonsterState::Hurt, CAlbumPlayer::CreatePlayerFromFile(L"dominatedunnaturals_Hurt"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\monster_dominatedunnaturals_Hurt.animation"));
			pMonster->SetDetectRange(250);
			pMonster->SetAttackRange(100);
			pMonster->SetAttackFrame(make_pair(2, 3));

			m_MonsterMap.insert(make_pair(pMonster->GetID(), pMonster));
			break;
		}
		}
	}

	// 벽 오브젝트 추가
	for (WallInfo* pWallInfo : m_StageInfo->vecWallInfo)
	{
		CWall* pWall = new CWall(pWallInfo->Name);
		pWall->SetLocation(pWallInfo->Pos);
		pWall->SetScale(pWallInfo->Size);
		CCollider* pWallCollider = new CCollider(pWallInfo->Name + L"_Col");
		pWallCollider->SetSize(pWall->GetScale());
		pWall->AddComponent(pWallCollider);
		pWall->RegisterBodyCollider(pWallCollider);
		CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pWall, LayerType::Object);
		pWall->AddComponent(new CSticker(pWallInfo->Name));

		m_WallMap.insert(make_pair(pWall->GetID(), pWall));
	}

	
	// NPC 오브젝트 추가
	for (NPCInfo* pNPCInfo : m_StageInfo->vecNPCInfo)
	{
		CNPC* pNPC = new CNPC(pNPCInfo->Name);
		pNPC->SetLocation(pNPCInfo->Pos);
		pNPC->SetScale(pNPCInfo->Size);
		CCollider* pNPCCollider = new CCollider(pNPCInfo->Name + L"_Col");
		pNPCCollider->SetSize(pNPC->GetScale());
		pNPC->AddComponent(pNPCCollider);
		pNPC->RegisterBodyCollider(pNPCCollider);
		CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pNPC, LayerType::Object);
		pNPC->AddComponent(new CSticker(pNPCInfo->Name));
		pNPC->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pNPCInfo->Name + L"_IdleAni"
			, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + pNPCInfo->IdleAnimation));
		pNPC->SetIdleAni(pNPCInfo->IdleAnimation);

		m_NPCMap.insert(make_pair(pNPC->GetID(), pNPC));
	}

	// 레벨 종료시 다시 추가되므로 벡터 클리어
	Delete_Vector(m_StageInfo->vecMonsterInfo);
	Delete_Vector(m_StageInfo->vecNPCInfo);
	Delete_Vector(m_StageInfo->vecWallInfo);

	// BGM이 있으면 재생
	if (!m_StageInfo->BGMPath.empty())
	{
		m_BGM = CSoundMgr::GetInst()->GetSound(m_StageInfo->BGMPath, L"\\music\\" + m_StageInfo->BGMPath);
		m_BGM->SetVolume(50);
		m_BGM->PlayToBGM(true);
	}

	m_hControlPanel = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_StageMakerPanel)
		, CEngine::GetInst()->GetMainWnd(), &StageMakerPanelProc);
	ShowWindow(m_hControlPanel, SW_SHOWNORMAL);
	m_hSelectedObj = GetDlgItem(m_hControlPanel, STATIC_SelectedObj);
	m_hUpperBoundSlide = GetDlgItem(m_hControlPanel, SLIDE_UpperBound);
	m_hUpperBoundStatic = GetDlgItem(m_hControlPanel, STATIC_UpperBound);
	SendMessage(m_hUpperBoundSlide, TBM_SETRANGE, TRUE, MAKELONG(0, m_StageInfo->StageSize.y));
	SendMessage(m_hUpperBoundSlide, TBM_SETPOS, TRUE, m_StageInfo->UpperBound);
	SetWindowText(m_hUpperBoundStatic, std::to_wstring(m_StageInfo->UpperBound).c_str());
}

void CStageMaker::Tick()
{
	CStage::Tick();
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::ESC) == Key_state::TAP)
	{
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"DungeonMaker"));
	}

	if (m_hSelectedObj != NULL)
	{
		if (GetSelectedObj() != nullptr)
			SetWindowText(m_hSelectedObj, (L"ID: " + std::to_wstring(GetSelectedObj()->GetID()) + L", Name: " + GetSelectedObj()->GetName()).c_str());
		else
			SetWindowText(m_hSelectedObj, L"(NONE)");
	}

	m_UpperBound = SendMessage(m_hUpperBoundSlide, TBM_GETPOS, 0, 0);
	SetWindowText(m_hUpperBoundStatic, std::to_wstring(m_UpperBound).c_str());
}

void CStageMaker::FinalTick()
{
	CStage::FinalTick();
}

void CStageMaker::Render()
{
	CStage::Render();

	CTextureMgr::GetInst()->DrawLine(Color(255, 255, 0, 0), 2
		, Vec2D(-10.f, m_StageInfo->StageSize.y - m_UpperBound), Vec2D(m_StageInfo->StageSize.x + 10, m_StageInfo->StageSize.y - m_UpperBound));
	CTextureMgr::GetInst()->DrawRect(Color(255, 255, 0, 0), 2, Vec2D(0, 0), m_StageInfo->StageSize);
	CTextMgr::GetInst()->Render(10, 10, m_StageInfo->StageName, Color(255, 0, 0, 0), true);

}

void CStageMaker::End()
{
	DestroyWindow(m_hControlPanel);
	m_hControlPanel = nullptr;
	m_hSelectedObj = nullptr;
	m_hUpperBoundSlide = nullptr;
	m_hUpperBoundStatic = nullptr;
	SetSelectedObj(nullptr);

	// 각각의 몬스터, 벽, NPC 맵으로부터 객체정보를 바탕으로 인포 벡터를 작성, 스테이지 인포를 만들어서 전달
	for (map<int, CMonster*>::iterator iter = m_MonsterMap.begin(); iter != m_MonsterMap.end(); ++iter)
	{
		MonsterInfo* desc = new MonsterInfo;
		desc->MonsterTemplate = iter->second->GetMonsterTemplate();
		desc->Name = iter->second->GetName();
		desc->pos = iter->second->GetLocation();

		m_StageInfo->vecMonsterInfo.push_back(desc);
	}
	m_MonsterMap.clear();

	for (map<int, CWall*>::iterator iter = m_WallMap.begin(); iter != m_WallMap.end(); ++iter)
	{
		WallInfo* desc = new WallInfo;
		desc->Name = iter->second->GetName();
		desc->Pos = iter->second->GetLocation();
		desc->Size = iter->second->GetScale();

		m_StageInfo->vecWallInfo.push_back(desc);
	}
	m_WallMap.clear();

	for (map<int, CNPC*>::iterator iter = m_NPCMap.begin(); iter != m_NPCMap.end(); ++iter)
	{
		NPCInfo* desc = new NPCInfo;
		desc->Name = iter->second->GetName();
		desc->Pos = iter->second->GetLocation();
		desc->Size = iter->second->GetScale();
		desc->IdleAnimation = iter->second->GetIdleAni();

		m_StageInfo->vecNPCInfo.push_back(desc);
	}
	m_NPCMap.clear();

	for (int i = 0; i < 4; ++i)
	{
		if (m_PortalArr[i] != nullptr)
		{
			m_StageInfo->arrPortalInfo[i].Location = m_PortalArr[i]->GetLocation();
		}
	}

	m_StageInfo->UpperBound = m_UpperBound;


	CStage::End();
}

void CStageMaker::EraseObjFromMap(int _ID)
{
	auto EraseObj = [_ID](auto& map)
		{
			auto iter = map.find(_ID);
			if (iter != map.end())
			{
				map.erase(iter);
				return true;
			}
			return false;
		};

	if (EraseObj(m_MonsterMap))
		return;
	if (EraseObj(m_WallMap))
		return;
	if (EraseObj(m_NPCMap))
		return;

}
