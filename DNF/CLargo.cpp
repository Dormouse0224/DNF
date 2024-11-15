#include "pch.h"
#include "CLargo.h"
#include "CAlbumPlayer.h"
#include "CLargo_Attack.h"
#include "CEngine.h"
#include "CLargo_Income.h"
#include "CLargo_Attack.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CDbgRender.h"
#include "CDummy.h"
#include "CCameraMgr.h"
#include "CLevelMgr.h"
#include "CLargo_Death.h"

CLargo::CLargo()
	: CObj(L"Largo")
	, m_Animation{}
	, m_bIncomeFin(false)
	, m_AttackState(nullptr)
{
	m_MaxHP = 10000;
	m_CurHP = 10000;

	SetLayerType(LayerType::Object);
	SetScale(Vec2D(150, 150));
	AddAnimation(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_incomingB_0"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_incomingB_0.animation"), LargoState::Income);
	AddAnimation(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_incomingB_1"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_incomingB_1.animation"), LargoState::Income);
	AddAnimation(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_death_0"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_death_0.animation"), LargoState::Death);
	AddAnimation(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_death_1"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_death_1.animation"), LargoState::Death);

	CFSM* pFSM = new CFSM(L"Largo_FSM");
	pFSM->AddState((int)LargoState::Income, new CLargo_Income);
	m_AttackState = new CLargo_Attack;
	pFSM->AddState((int)LargoState::Attack, m_AttackState);
	pFSM->AddState((int)LargoState::Death, new CLargo_Death);
	AddComponent(pFSM);

	SetRigidBody(new CRigidBody(L"Largo_RB"));

	CCollider* pCol = new CCollider(L"Largo_Col");
	RegisterBodyCollider(pCol);
	AddComponent(pCol);

	// 몬스터 HUD 등록
	m_HUD = new CDummy(L"hud");
	m_HUD->SetLocation(CCameraMgr::GetInst()->GetCameraPos() + Vec2D(10, 50));
	m_HUD->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_monsterhp_0"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_monsterhp_0.animation"));
	m_HPTex = CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_monsterhp_1"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_monsterhp_1.animation", Vec2D(30, 17));
	m_HUD->AddComponent(m_HPTex);
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_HUD, LayerType::Near);
}

CLargo::~CLargo()
{
	CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(LayerType::Near, m_HUD->GetID());
}

void CLargo::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CLargo::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CLargo::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CLargo::Begin()
{
}

void CLargo::Tick()
{
	// hud 위치 갱신
	m_HUD->SetLocation(CCameraMgr::GetInst()->GetCameraPos() + Vec2D(10, 50));

	// hp, mp 퍼센티지 갱신
	m_HPTex->SetRenderPercentH(m_CurHP / m_MaxHP);

	// 바닥 위치 갱신
	Vec2D gp(GetLocation().x + (GetScale().x / 2.f), GetLocation().y + GetScale().y);
	if (GetRigidBody() != nullptr)
	{
		RenewGroundPos(gp - Vec2D(0.f, GetRigidBody()->GetAirborneHeight()));
	}
	else
	{
		RenewGroundPos(gp);
	}

	// 그라운드 포지션 디버그 렌더링
	CDbgRender::GetInst()->AddDbgRender(DbgRenderShape::Circle, GetGroundPos() - Vec2D(5, 5)
		, Vec2D(10, 10), 0, Color(255, 0, 0, 255));
}

void CLargo::Render()
{
	if (GetState() == (int)LargoState::Attack)
	{
		m_AttackState->Render();
	}
	else
	{
		for (int i = 0; i < m_Animation[GetState()].size(); ++i)
		{
			m_Animation[GetState()][i]->Render(this, m_bFallowCam, m_bLookLeft);
		}
	}

	
	if (GetState() == (int)LargoState::Income && m_Animation[GetState()][0]->GetCurSceneNum() == m_Animation[GetState()][0]->GetFinal())
	{
		m_bIncomeFin = true;
	}
	
}

void CLargo::AddAnimation(CAlbumPlayer* _AP, LargoState _st)
{
	m_Animation[(int)_st].push_back(_AP);
	AddComponent(_AP);
}
