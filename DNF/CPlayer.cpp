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
#include "CDummy.h"
#include "CCameraMgr.h"
#include "CLevelMgr.h"
#include "CSoundMgr.h"
#include "CSound.h"

#include "CPlayer_Idle.h"
#include "CPlayer_Walk.h"
#include "CPlayer_Run.h"
#include "CPlayer_Jump.h"
#include "CPlayer_Attack.h"
#include "CPlayer_Death.h"


CPlayer::CPlayer()
	: CObj(L"Player")
	, m_YogiGauge(0.f)
	, m_Avatar{}
	, m_AttackState(nullptr)
	, m_CurAvatarCode{ "0000", "0000" , "0000" , "0000" , "0000" , "0000" }
	, m_PrevAvatarCode{ "0000", "0000" , "0000" , "0000" , "0000" , "0000" }
	, m_HUD(nullptr)
	, m_HPTex(nullptr)
	, m_MPTex(nullptr)
{
	m_MaxHP = 1000;
	m_CurHP = 1000;
	m_MaxMP = 1000;
	m_CurMP = 1000;

	m_HPRegen = m_MaxHP * 0.01f;
	m_MPRegen = m_MaxMP * 0.1f;

	// 스킬 리소스 미리 로드
	CTextureMgr::PreloadFromFile(L"PlayerResources.txt");
	CTextureMgr::PreloadAvatar(L"AvatarResources.txt");

	// 상태별 모션 애니메이션
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
	AddAnimation(PlayerState::Death, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Down"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Down.animation"));
	CAlbumPlayer::CreateAvatar(L"archer_skin_00_Down", m_CurAvatarCode, m_Avatar[(int)PlayerState::Death]);

	// 상태 등록
	CFSM* pFSM = new CFSM(L"Player_FSM");
	pFSM->AddState((int)PlayerState::Idle, new CPlayer_Idle(L"Player_Idle"));
	pFSM->AddState((int)PlayerState::Walk, new CPlayer_Walk(L"Player_Walk"));
	pFSM->AddState((int)PlayerState::Run, new CPlayer_Run(L"Player_Run"));
	pFSM->AddState((int)PlayerState::Jump, new CPlayer_Jump(L"Player_Jump"));
	m_AttackState = new CPlayer_Attack(L"Player_Attack");
	pFSM->AddState((int)PlayerState::Attack, m_AttackState);
	pFSM->AddState((int)PlayerState::Death, new CPlayer_Death(L"Player_Death"));
	AddComponent(pFSM);

	// 강체 등록
	SetRigidBody(new CRigidBody(L"Player_RB"));

	// 충돌체 등록
	CCollider* pCol = new CCollider(L"Player_Col");
	RegisterBodyCollider(pCol);
	AddComponent(pCol);

	// 플레이어 HUD 등록
	m_HUD = new CDummy(L"hud");
	m_HUD->SetLocation(CCameraMgr::GetInst()->GetCameraPos() + Vec2D(347, 515));
	m_HUD->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_skillwindow"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_skillwindow.animation"));
	m_HUD->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_hpmpvessel"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_hpmpvessel.animation", Vec2D(25, 20)));
	m_HPTex = CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_hp"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_hp.animation", Vec2D(25, 20));
	m_HUD->AddComponent(m_HPTex);
	m_HUD->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_hpmpvessel"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_hpmpvessel.animation", Vec2D(319, 20)));
	m_MPTex = CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_mp"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_mp.animation", Vec2D(319, 20));
	m_HUD->AddComponent(m_MPTex);
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_HUD, LayerType::Near);
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

void CPlayer::GiveDamage(int _dmg)
{
	if (m_Immune)
		return;
	CObj::GiveDamage(_dmg);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 4);
	CSoundMgr::GetInst()->GetSound(L"ar_dmg_0" + std::to_wstring(dis(gen))
		, L"\\sound\\sounds_char_archer\\ar_dmg_0" + std::to_wstring(dis(gen)) + L".ogg")->Play();
}

void CPlayer::Begin()
{
}

void CPlayer::Tick()
{
	// hud 위치 갱신
	m_HUD->SetLocation(CCameraMgr::GetInst()->GetCameraPos() + Vec2D(347, 525));

	// hp, mp 재생
	m_CurHP = min(m_HPRegen * CTimeMgr::GetInst()->GetDeltaTime() + m_CurHP, m_MaxHP);
	m_CurMP = min(m_MPRegen * CTimeMgr::GetInst()->GetDeltaTime() + m_CurMP, m_MaxMP);

	// hp, mp 퍼센티지 갱신
	m_HPTex->SetRenderPercent(m_CurHP / m_MaxHP);
	m_MPTex->SetRenderPercent(m_CurMP / m_MaxMP);

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
		{
			m_PrevAvatarCode[i] != m_CurAvatarCode[i];
			return true;
		}
	}
	return false;
}
