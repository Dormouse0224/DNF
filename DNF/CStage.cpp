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


CStage::CStage(wstring _name)
	: CLevel(_name)
	, m_StageInfo(nullptr)
	, m_BGM(nullptr)
	, m_UpperBound(0)
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
			CCollider* pMonCollider = new CCollider(pMonsterInfo->Name + L"_Col");
			pMonCollider->SetSize(pMonster->GetScale());
			pMonster->AddComponent(pMonCollider);
			pMonster->RegisterBodyCollider(pMonCollider);
			CCollider* pAttCollider = new CCollider(pMonsterInfo->Name + L"_AttCol");
			pAttCollider->SetSize(Vec2D(240, 120));
			pAttCollider->SetOffset(Vec2D(-40, 15));
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
		pNPC->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pNPCInfo->Name + L"_IdleAni"
			, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + pNPCInfo->IdleAnimation));
		pNPC->SetIdleAni(pNPCInfo->IdleAnimation);
	}

	// 플레이어 추가
	CPlayer* pPlayer = new CPlayer();
	AddObject(pPlayer, LayerType::Object);
	SetPlayer(pPlayer);


	// BGM이 있으면 재생
	if (!m_StageInfo->BGMPath.empty())
	{
		m_BGM = CSoundMgr::GetInst()->GetSound(m_StageInfo->BGMPath, L"\\music\\" + m_StageInfo->BGMPath);
		m_BGM->PlayToBGM(true);
	}
}

void CStage::Tick()
{
	if (GetPlayer()->GetGroundPos().x < 0 || GetPlayer()->GetGroundPos().x > m_StageInfo->StageSize.x)
		GetPlayer()->SetGroundPos(Vec2D(min(max(GetPlayer()->GetGroundPos().x, 0), m_StageInfo->StageSize.x), GetPlayer()->GetGroundPos().y));
	if (GetPlayer()->GetGroundPos().y < m_StageInfo->StageSize.y - m_UpperBound || GetPlayer()->GetGroundPos().y > m_StageInfo->StageSize.y)
		GetPlayer()->SetGroundPos(Vec2D(GetPlayer()->GetGroundPos().x, min(max(GetPlayer()->GetGroundPos().y, m_StageInfo->StageSize.y - m_UpperBound), m_StageInfo->StageSize.y)));

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
}

