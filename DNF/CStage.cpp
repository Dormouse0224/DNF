#include "pch.h"
#include "CStage.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CPortal.h"
#include "CMonster.h"
#include "CWall.h"
#include "CNPC.h"
#include "CLevelMgr.h"
#include "CCollider.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CPlayer.h"
#include "CCollisionMgr.h"
#include "CFSM.h"
#include "CMonster_Attack.h"
#include "CMonster_Idle.h"
#include "CMonster_Hurt.h"
#include "CMonster_Move.h"
#include "CDbgRender.h"
#include "CNPCCallback.h"
#include "CTimeMgr.h"
#include "CDummy.h"
#include "CCameraMgr.h"


CStage::CStage(wstring _name)
	: CLevel(_name)
	, m_StageInfo(nullptr)
	, m_BGM(nullptr)
	, m_UpperBound(0)
	, m_bStageClear(false)
	, m_ClearTimer(10)
	, m_Effect(nullptr)
	, m_EffectOwner(nullptr)
	, m_PortalVec{}
{
}

CStage::~CStage()
{
	if (m_StageInfo)
		delete m_StageInfo;
}

void CStage::Begin()
{
	SetCameraFollowPlayer(true);

	// 배경 객체 생성 후 애니메이션 입력
	CBackground* pBackground = new CBackground(m_StageInfo->StageName + L"_Background");
	pBackground->SetScale(m_StageInfo->StageSize);
	AddObject(pBackground, LayerType::Background);
	for (int i = 0; i < m_StageInfo->vecBGA.size(); ++i)
	{
		pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"BGA_" + std::to_wstring(i)
			, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + m_StageInfo->vecBGA[i]));
	}
	// 타일 추가
	if (!m_StageInfo->Tile1Path.empty())
	{
		for (int i = 0; i < (m_StageInfo->StageSize.x - m_StageInfo->Tile1Pos.x) / m_StageInfo->Tile1Size.x; ++i)
		{
			CAlbumPlayer* pAP = CAlbumPlayer::CreatePlayerFromFile(L"Tile1_" + std::to_wstring(i), CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\" + m_StageInfo->Tile1Path, m_StageInfo->Tile1Pos + Vec2D(m_StageInfo->Tile1Size.x * i, 0.f));
			pAP->SetCurrentScene(i % (pAP->GetFinal() + 1));
			pBackground->AddComponent(pAP);
		}
	}
	if (!m_StageInfo->Tile2Path.empty())
	{
		for (int j = 0; j < (m_StageInfo->StageSize.y - m_StageInfo->Tile2Pos.y) / m_StageInfo->Tile2Size.y; ++j)
		{
			for (int i = 0; i < (m_StageInfo->StageSize.x - m_StageInfo->Tile2Pos.x) / m_StageInfo->Tile2Size.x; ++i)
			{
				CAlbumPlayer* pAP = CAlbumPlayer::CreatePlayerFromFile(L"Tile2_" + std::to_wstring(j) + L"_" + std::to_wstring(i), CEngine::GetInst()->GetResourcePathW()
					+ L"\\animation\\" + m_StageInfo->Tile2Path, m_StageInfo->Tile2Pos + Vec2D(m_StageInfo->Tile2Size.x * i, m_StageInfo->Tile2Size.y * j));
				pAP->SetCurrentScene(i % (pAP->GetFinal() + 1));
				pBackground->AddComponent(pAP);
			}
		}
	}


	// 플레이어 영역 upperbound 세팅
	m_UpperBound = m_StageInfo->UpperBound;

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
			m_PortalVec.push_back(pPortal);
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
			AddObject(pMonster, LayerType::Object);
			pMonster->SetLocation(pMonsterInfo->pos);
			pMonster->SetScale(Vec2D(100, 140));
			pMonster->SetMaxHP(1000);
			pMonster->SetCurHP(1000);
			CCollider* pMonCollider = new CCollider(pMonsterInfo->Name + L"_Col");
			pMonCollider->SetSize(pMonster->GetScale());
			pMonster->AddComponent(pMonCollider);
			pMonster->RegisterBodyCollider(pMonCollider);
			CCollider* pAttCollider = new CCollider(pMonsterInfo->Name + L"_AttCol");
			pAttCollider->SetSize(Vec2D(240, 120));
			pAttCollider->SetOffset(Vec2D(-40, 15));
			pAttCollider->SetActiveState(false);
			pMonster->AddComponent(pAttCollider);
			pMonster->SetAttackCol(pAttCollider);
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
			CFSM* pFSM = new CFSM(pMonsterInfo->Name + L"_FSM");
			pFSM->AddState((int)MonsterState::Idle, new CMonster_Idle(L"Monster_Idle"));
			pFSM->AddState((int)MonsterState::Move, new CMonster_Move(L"Monster_Move"));
			pFSM->AddState((int)MonsterState::Attack, new CMonster_Attack(L"Monster_Attack"));
			pFSM->AddState((int)MonsterState::Hurt, new CMonster_Hurt(L"Monster_Hurt"));
			pMonster->AddComponent(pFSM);
			break;
		}
		}
	}
	SetMonsterCount(m_StageInfo->vecMonsterInfo.size());
	

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
		AddObject(pWall, LayerType::Object);
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
		AddObject(pNPC, LayerType::Object);
		CAlbumPlayer* pAP = CAlbumPlayer::CreatePlayerFromFile(pNPCInfo->Name + L"_IdleAni"
			, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + pNPCInfo->IdleAnimation);
		pNPC->AddComponent(pAP);
		pNPC->SetIdlePlayer(pAP);
		pNPC->SetIdleAni(pNPCInfo->IdleAnimation);
		pNPC->SetCallbackIndex(pNPCInfo->CallbackIndex);
		if (pNPCInfo->CallbackIndex == 1)
		{
			pNPC->SetreactCallback(&SelectDungeonCallback);
		}
		else if (pNPCInfo->CallbackIndex == 2)
		{
			pNPC->SetreactCallback(&TeleportCallback);
			pNPC->SetTeleportDest(pNPCInfo->TeleportDest);
		}
	}

	// 플레이어 추가
	CPlayer* pPlayer = new CPlayer();
	AddObject(pPlayer, LayerType::Object);
	SetPlayer(pPlayer);
	pPlayer->SetGroundPos(Vec2D(m_StageInfo->StageSize.x / 2, m_StageInfo->StageSize.y - (m_StageInfo->UpperBound / 2)));


	// BGM이 있으면 재생
	if (!m_StageInfo->BGMPath.empty())
	{
		m_BGM = CSoundMgr::GetInst()->GetSound(m_StageInfo->BGMPath, L"\\music\\" + m_StageInfo->BGMPath);
		m_BGM->PlayToBGM(true);
	}
}

void CStage::Tick()
{
	if (GetPlayer())
	{
		if (GetPlayer()->GetGroundPos().x < 0 || GetPlayer()->GetGroundPos().x > m_StageInfo->StageSize.x)
			GetPlayer()->SetGroundPos(Vec2D(min(max(GetPlayer()->GetGroundPos().x, 0), m_StageInfo->StageSize.x), GetPlayer()->GetGroundPos().y));
		if (GetPlayer()->GetGroundPos().y < m_StageInfo->StageSize.y - m_UpperBound || GetPlayer()->GetGroundPos().y > m_StageInfo->StageSize.y)
			GetPlayer()->SetGroundPos(Vec2D(GetPlayer()->GetGroundPos().x, min(max(GetPlayer()->GetGroundPos().y, m_StageInfo->StageSize.y - m_UpperBound), m_StageInfo->StageSize.y)));
	}

	// 스테이지 클리어
	if (GetMonsterCount() < 1)
	{
		for (int i = 0; i < m_PortalVec.size(); ++i)
		{
			m_PortalVec[i]->SetActive(true);
		}
		if (m_StageInfo->StageType == StageType::FINAL && m_bStageClear == false)
		{
			m_bStageClear = true;
			CDummy* pEff0 = new CDummy(L"Epicbeam");
			AddObject(pEff0, LayerType::Far);
			pEff0->SetLocation(GetPlayer()->GetLocation());
			m_Effect = CAlbumPlayer::CreatePlayerFromFile(L"ui_effect_epicbeam"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_effect_epicbeam.animation");
			m_EffectOwner = pEff0;
			pEff0->AddComponent(m_Effect);

			CDummy* pEff1 = new CDummy(L"Reward");
			AddObject(pEff1, LayerType::Far);
			pEff1->SetLocation(CCameraMgr::GetInst()->GetCameraPos() + Vec2D(533, 150));
			CAlbumPlayer* pAP = CAlbumPlayer::CreatePlayerFromFile(L"ui_effect_reward"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_effect_reward.animation");
			pEff1->SetFallowCam(true);
			pEff1->AddComponent(pAP);
		}
	}
	if (m_bStageClear)
		m_ClearTimer -= CTimeMgr::GetInst()->GetDeltaTime();
	if (m_ClearTimer < 0)
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"SeriaRoom"));
	if (m_Effect)
	{
		if (m_Effect->GetCurSceneNum() == m_Effect->GetFinal())
			CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(LayerType::Far, m_EffectOwner->GetID());
	}

	CCollisionMgr::GetInst()->AddCollisionLayer(LayerType::Object, LayerType::Object);

	CLevel::Tick();

}

void CStage::FinalTick()
{
	CLevel::FinalTick();
}

void CStage::Render()
{
	CDbgRender::GetInst()->AddDbgRender(DbgRenderShape::Line, Vec2D(-10.f, m_StageInfo->StageSize.y - m_UpperBound)
		, Vec2D(m_StageInfo->StageSize.x + 10, m_StageInfo->StageSize.y - m_UpperBound), 0, Color(255, 255, 0, 0));
	CDbgRender::GetInst()->AddDbgRender(DbgRenderShape::Rectangle
		, Vec2D(0, 0), m_StageInfo->StageSize, 0, Color(255, 255, 0, 0));
	CLevel::Render();
}

void CStage::End()
{
	ClearAll();
	m_bStageClear = false;
	m_ClearTimer = 10;
	m_Effect = nullptr;
	m_EffectOwner = nullptr;
}

