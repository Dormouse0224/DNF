#include "pch.h"
#include "CPlayer.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CFSM.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CDbgRender.h"
#include "CTimeMgr.h"
#include "CTextureMgr.h"

#include "CPlayer_Idle.h"
#include "CPlayer_Walk.h"
#include "CPlayer_Run.h"
#include "CPlayer_Jump.h"
#include "CPlayer_Attack.h"


CPlayer::CPlayer()
	: CObj(L"Player")
	, m_YogiGauge(0.f)
	, m_Avatar{}
	, m_AttackState(nullptr)
	, m_CurAvatarCode{ "0000", "0000" , "0000" , "0000" , "0000" , "0000" }
	, m_PrevAvatarCode{ "0000", "0000" , "0000" , "0000" , "0000" , "0000" }
{
	// 스킬 리소스 미리 로드
	CTextureMgr::PreloadFromFile(L"PlayerSkillResources.txt");

	for (int i = 0; i < (int)PlayerState::END; ++i) {
		m_Avatar[i] = std::vector<CAlbumPlayer*>(25, nullptr); // 각 벡터의 크기를 25로 설정하고 nullptr로 초기화
	}
	SetLayerType(LayerType::Object);
	SetScale(Vec2D(40, 95));
	AddAnimation(PlayerState::Idle, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Idle.animation"));
	CAlbumPlayer::CreateAvatar(L"archer_skin_00_Idle", m_CurAvatarCode, m_Avatar[(int)PlayerState::Idle]);
	AddAnimation(PlayerState::Walk, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Walk"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Walk.animation"));
	CAlbumPlayer::CreateAvatar(L"archer_skin_00_Walk", m_CurAvatarCode, m_Avatar[(int)PlayerState::Walk]);
	AddAnimation(PlayerState::Run, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Run"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Run.animation"));
	CAlbumPlayer::CreateAvatar(L"archer_skin_00_Run", m_CurAvatarCode, m_Avatar[(int)PlayerState::Run]);
	AddAnimation(PlayerState::Jump, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Jump"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Jump.animation"));
	CAlbumPlayer::CreateAvatar(L"archer_skin_00_Jump", m_CurAvatarCode, m_Avatar[(int)PlayerState::Jump]);

	CFSM* pFSM = new CFSM(L"Player_FSM");
	pFSM->AddState((int)PlayerState::Idle, new CPlayer_Idle(L"Player_Idle"));
	pFSM->AddState((int)PlayerState::Walk, new CPlayer_Walk(L"Player_Walk"));
	pFSM->AddState((int)PlayerState::Run, new CPlayer_Run(L"Player_Run"));
	pFSM->AddState((int)PlayerState::Jump, new CPlayer_Jump(L"Player_Jump"));
	m_AttackState = new CPlayer_Attack(L"Player_Attack");
	pFSM->AddState((int)PlayerState::Attack, m_AttackState);
	AddComponent(pFSM);

	SetRigidBody(new CRigidBody(L"Player_RB"));

	CCollider* pCol = new CCollider(L"Player_Col");
	RegisterBodyCollider(pCol);
	AddComponent(pCol);
}

CPlayer::~CPlayer()
{
	for (int i = 0; i < (int)PlayerState::END; ++i)
	{
		for (int j = 0; j < 25; ++j)
		{
			delete m_Avatar[i][j];
		}
		for (int j = 0; j < m_Animation[i].size(); ++j)
		{
			delete m_Animation[i][j];
		}
	}
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
		RenewGroundPos(gp - Vec2D(0.f, GetRigidBody()->GetAirborneHeight()));
	}
	else
	{
		RenewGroundPos(gp);
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

	int state = GetState();
	for (int i = 0; i < m_Animation[state].size(); ++i)
	{
		m_Animation[state][i]->FinalTick();
	}
	for (int i = 0; i < 25; ++i)
	{
		if (m_Avatar[state][i])
			m_Avatar[state][i]->FinalTick();
	}

	if (CheckAvatarCodeChange())
	{
		CAlbumPlayer::CreateAvatar(L"archer_skin_00_Idle", m_CurAvatarCode, m_Avatar[(int)PlayerState::Idle]);
		CAlbumPlayer::CreateAvatar(L"archer_skin_00_Walk", m_CurAvatarCode, m_Avatar[(int)PlayerState::Walk]);
		CAlbumPlayer::CreateAvatar(L"archer_skin_00_Run", m_CurAvatarCode, m_Avatar[(int)PlayerState::Run]);
		CAlbumPlayer::CreateAvatar(L"archer_skin_00_Jump", m_CurAvatarCode, m_Avatar[(int)PlayerState::Jump]);
	}
}

void CPlayer::Render()
{
	if (GetState() == (int)PlayerState::Attack)
	{
		m_AttackState->Render();
	}
	else
	{
		int state = GetState();
		for (int i = 0; i < m_Animation[state].size(); ++i)
		{
			m_Animation[state][i]->Render(this, false, m_bLookLeft);
		}
		for (int i = 0; i < 25; ++i)
		{
			if (m_Avatar[state][i])
				m_Avatar[state][i]->Render(this, false, m_bLookLeft);
		}
	}
}

void CPlayer::AddAnimation(PlayerState _state, CAlbumPlayer* _animation)
{
	m_Animation[(int)_state].push_back(_animation);
	//AddComponent(_animation);
}

bool CPlayer::CheckAvatarCodeChange()
{
	for (int i = 0; i < 6; ++i)
	{
		if (m_CurAvatarCode[i] != m_PrevAvatarCode[i])
			return true;
	}
	return false;
}
