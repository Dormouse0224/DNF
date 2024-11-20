#include "pch.h"
#include "CLargo_Pattern.h"
#include "CCameraMgr.h"
#include "CObj.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CDummy.h"
#include "CEngine.h"
#include "CAlbumPlayer.h"
#include "CTextureMgr.h"
#include "CSound.h"
#include "CSoundMgr.h"


CLargo_Pattern::CLargo_Pattern()
	: CState(L"Largo_Pattern")
	, m_PatternTimer(5)
	, m_bClear(false)
	, m_VoiceNum(0)
	, m_Effect(nullptr)
	, m_IdleAnimation(nullptr)
	, m_IdleAnimation1(nullptr)
{
	m_IdleAnimation = CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Idle_0"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Idle_0.animation");
	m_IdleAnimation1 = CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Idle_1"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Idle_1.animation");
}

CLargo_Pattern::~CLargo_Pattern()
{
	delete m_IdleAnimation;
	delete m_IdleAnimation1;
}

void CLargo_Pattern::Enter()
{
	CPlayer* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();

	GetOwnerObj()->SetState((int)LargoState::Pattern);
	GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));
	pPlayer->SetImmune(true);
	pPlayer->SetYogiGauge(0);
	GetOwnerObj()->SetImmune(true);
	CCameraMgr::GetInst()->SetAlpha(150);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 5);
	m_VoiceNum = dis(gen);

	CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_skill_16_" + std::to_wstring(m_VoiceNum)
		, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_skill_16_" + std::to_wstring(m_VoiceNum) + L".ogg")->Play();
}

void CLargo_Pattern::FinalTick()
{
	CPlayer* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();
	pPlayer->SetYogiGauge(0);
	if (m_PatternTimer > 0)
	{
		m_PatternTimer -= CTimeMgr::GetInst()->GetDeltaTime();
		if (!m_bStop)
		{
			m_BarPos = (CEngine::GetInst()->GetResolution() / 2) + Vec2D(abs(fmod(m_PatternTimer, 2.f) - 1.f) * 200.f - 100.f, 0.f);
		}

		// 절단 패턴 도중
		if (CKeyMgr::GetInst()->GetKeyState(Keyboard::X) == Key_state::TAP)
		{
			m_bStop = true;
			if (m_BarPos.x >= (CEngine::GetInst()->GetResolution() / 2).x - 20 && m_BarPos.x <= (CEngine::GetInst()->GetResolution() / 2).x + 20)
				m_bClear = true;
		}

	}
	else
	{
		if (m_bClear)
		{
			pPlayer->SetImmune(false);
			GetOwnerObj()->SetImmune(false);
			GetFSM()->ChangeState((int)LargoState::Attack);

			CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_skill_17_1"
				, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_skill_17_1.ogg")->Play();
		}
		else
		{
			// 절단패턴 파훼 실패 시
			pPlayer->SetImmune(false);
			pPlayer->GiveDamage(pPlayer->GetMaxHP() * 1.1f);
			GetOwnerObj()->SetImmune(false);
			CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_skill_16_" + std::to_wstring(m_VoiceNum + 5)
				, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_skill_16_" + std::to_wstring(m_VoiceNum + 5) + L".ogg")->Play();

			m_Effect = new CDummy(L"Slash");
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_Effect, LayerType::Far);
			m_Effect->SetLocation(CCameraMgr::GetInst()->GetCameraPos() + (CEngine::GetInst()->GetResolution() / 2));
			m_Effect->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_effect_pattern_0"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_effect_pattern_0.animation"));
			m_Effect->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_effect_pattern_1"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_effect_pattern_1.animation", Vec2D(-200, -200)));
			m_Effect->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_effect_pattern_1"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_effect_pattern_1.animation", Vec2D(200, 200)));
			m_Effect->SetFallowCam(true);
			m_Effect->SetSuicideTimer(2);

			GetFSM()->ChangeState((int)LargoState::Attack);
		}
	}
}

void CLargo_Pattern::Exit()
{
	CCameraMgr::GetInst()->SetAlpha(0);
	m_PatternTimer = 5;
	m_bStop = false;
	m_bClear = false;
}

void CLargo_Pattern::Render()
{
	m_IdleAnimation->Render(GetOwnerObj(), false, GetOwnerObj()->IsLookLeft());
	m_IdleAnimation1->Render(GetOwnerObj(), false, GetOwnerObj()->IsLookLeft());

	CTextureMgr::GetInst()->DrawLine(Color(255, 255, 128, 0), 3, (CEngine::GetInst()->GetResolution() / 2) - Vec2D(100, 0)
		, (CEngine::GetInst()->GetResolution() / 2) + Vec2D(100, 0), true);
	CTextureMgr::GetInst()->DrawLine(Color(255, 0, 255, 0), 3, (CEngine::GetInst()->GetResolution() / 2) - Vec2D(20, 0)
		, (CEngine::GetInst()->GetResolution() / 2) + Vec2D(20, 0), true);

	CTextureMgr::GetInst()->DrawLine(Color(255, 255, 255, 255), 3, m_BarPos - Vec2D(0, 10)
		, m_BarPos + Vec2D(0, 10), true);
}