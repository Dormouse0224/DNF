#include "pch.h"
#include "CPlayer_Death.h"
#include "CDummy.h"
#include "CLevelMgr.h"
#include "CCameraMgr.h"
#include "CEngine.h"
#include "CTimeMgr.h"
#include "CAlbumPlayer.h"
#include "CKeyMgr.h"
#include "CRigidBody.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CPlayer.h"

CPlayer_Death::CPlayer_Death(wstring _name)
	: CState(_name)
	, m_Countdown(10)
	, m_CountdownObj(nullptr)
	, m_CountdownTex(nullptr)
{
}

CPlayer_Death::~CPlayer_Death()
{
}

void CPlayer_Death::Enter()
{
	((CPlayer*)GetOwnerObj())->StateAnimationInit(PlayerState::Death);
	GetOwnerObj()->SetImmune(true);
	GetOwnerObj()->SetState((int)PlayerState::Death);
	CCameraMgr::GetInst()->SetAlpha(150);
	GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));
	GetOwnerObj()->GetRigidBody()->Jump(300);
	m_Countdown = 10.f;

	vector<CAlbumPlayer*>& vec0 = ((CPlayer*)GetOwnerObj())->GetAnimationVec(PlayerState::Death);
	for (int i = 0; i < vec0.size(); ++i)
	{
		if (vec0[i])
			vec0[i]->SetCurrentScene(0);
	}
	vector<CAlbumPlayer*>& vec1 = ((CPlayer*)GetOwnerObj())->GetAvatarVec(PlayerState::Death);
	for (int i = 0; i < vec1.size(); ++i)
	{
		if (vec1[i])
			vec1[i]->SetCurrentScene(0);
	}

	m_CountdownObj = new CDummy(L"Countdown");
	m_CountdownObj->SetLocation(CCameraMgr::GetInst()->GetCameraPos() + Vec2D(510, 280));
	m_CountdownTex = CAlbumPlayer::CreatePlayerFromFile(L"ui_countdown"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_countdown.animation");
	m_CountdownObj->AddComponent(m_CountdownTex);
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_CountdownObj, LayerType::Near);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 3);
	CSoundMgr::GetInst()->GetSound(L"ar_die_0" + std::to_wstring(dis(gen))
		, L"\\sound\\sounds_char_archer\\ar_die_0" + std::to_wstring(dis(gen)) + L".ogg")->Play();
}

void CPlayer_Death::FinalTick()
{
	m_CountdownObj->SetLocation(CCameraMgr::GetInst()->GetCameraPos() + Vec2D(510, 280));
	m_Countdown -= CTimeMgr::GetInst()->GetDeltaTime();
	m_CountdownTex->SetCurrentScene((int)m_Countdown);

	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::X) == Key_state::TAP && GetOwnerObj()->GetRigidBody()->GetAirborne() == false)
	{
		GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Idle));
	}

	if (m_Countdown < 0)
	{
		CCameraMgr::GetInst()->SetAlpha(255);
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"SeriaRoom"));
	}
	
}

void CPlayer_Death::Exit()
{
	GetOwnerObj()->SetImmune(false);
	CCameraMgr::GetInst()->SetAlpha(0);
	CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(LayerType::Near, m_CountdownObj->GetID());
	GetOwnerObj()->SetCurHP(GetOwnerObj()->GetMaxHP());
	GetOwnerObj()->SetCurMP(GetOwnerObj()->GetMaxMP());
}
