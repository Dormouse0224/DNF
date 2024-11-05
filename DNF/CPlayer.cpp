#include "pch.h"
#include "CPlayer.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CFSM.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CDbgRender.h"
#include "CTimeMgr.h"

#include "CPlayer_Idle.h"
#include "CPlayer_Walk.h"
#include "CPlayer_Run.h"
#include "CPlayer_Jump.h"


CPlayer::CPlayer()
	: CObj(L"Player")
	, m_YogiGauge(0.f)
{
	SetLayerType(LayerType::Object);
	SetScale(Vec2D(40, 95));
	AddAnimation(PlayerState::Idle, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Idle.animation"));
	AddAnimation(PlayerState::Walk, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Walk"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Walk.animation"));
	AddAnimation(PlayerState::Run, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Run"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Run.animation"));
	AddAnimation(PlayerState::Jump, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Jump"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Jump.animation"));

	CFSM* pFSM = new CFSM(L"Player_FSM");
	pFSM->AddState((int)PlayerState::Idle, new CPlayer_Idle(L"Player_Idle"));
	pFSM->AddState((int)PlayerState::Walk, new CPlayer_Walk(L"Player_Walk"));
	pFSM->AddState((int)PlayerState::Run, new CPlayer_Run(L"Player_Run"));
	pFSM->AddState((int)PlayerState::Jump, new CPlayer_Jump(L"Player_Jump"));
	AddComponent(pFSM);

	SetRigidBody(new CRigidBody(L"Player_RB"));

	CCollider* pCol = new CCollider(L"Player_Col");
	RegisterBodyCollider(pCol);
	AddComponent(pCol);
}

CPlayer::~CPlayer()
{
}

void CPlayer::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CPlayer::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CPlayer::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CPlayer::Begin()
{
}

void CPlayer::Tick()
{
	// 바닥 위치 갱신
	Vec2D gp(GetLocation().x + (GetScale().x / 2.f), GetLocation().y + GetScale().y);
	if (GetRigidBody() != nullptr)
	{
		SetGroundPos(gp - Vec2D(0.f, GetRigidBody()->GetAirborneHeight()));
	}
	else
	{
		SetGroundPos(gp);
	}

	// 그라운드 포지션 디버그 렌더링
	CDbgRender::GetInst()->AddDbgRender(DbgRenderShape::Circle, GetGroundPos() - Vec2D(5, 5)
		, Vec2D(10, 10), 0, Color(255, 0, 0, 255));

	// 바라보는 방향 갱신
	if (GetRigidBody()->GetSpeed().x < 0)
	{
		m_bLookLeft = true;
	}
	else if (GetRigidBody()->GetSpeed().x > 0)
	{
		m_bLookLeft = false;
	}
	
	// 요기 게이지 감소
	if (m_YogiGauge > 0)
	{
		m_YogiGauge -= CTimeMgr::GetInst()->GetDeltaTime() * 10;
		if (m_YogiGauge < 0)
			m_YogiGauge = 0;
	}
}

void CPlayer::Render()
{
	int state = GetState();
	for (int i = 0; i < m_Animation[state].size(); ++i)
	{
		m_Animation[state][i]->Render(this, false, m_bLookLeft);
	}
}

void CPlayer::AddAnimation(PlayerState _state, CAlbumPlayer* _animation)
{
	m_Animation[(int)_state].push_back(_animation);
	AddComponent(_animation);
}