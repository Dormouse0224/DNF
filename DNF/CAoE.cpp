#include "pch.h"
#include "CAoE.h"
#include "CTimeMgr.h"
#include "CTextureMgr.h"
#include "CLevelMgr.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CSoundMgr.h"
#include "CSound.h"

CAoE::CAoE(wstring _name)
	: CObj(_name)
	, m_Type(AoEType::END)
	, m_Pos(0, 0)
	, m_Param1(0)
	, m_Param2(0)
	, m_CastTime(0.f)
	, m_IsVert(false)
	, m_Timer(0.f)
	, m_IsCastFin(false)
	, m_bInit(false)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(-20, 20);
	m_RingSlash = CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_effect_ringslash_dodge", CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\boss_unshackled_effect_ringslash_dodge.animation");
	m_RingSlash->SetAngle(dis(gen));

	m_ScreenEffect[(int)ScreenEffect::SlashStart] = CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_effect_screenattack_slashstart_dodge"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_effect_screenattack_slashstart_dodge.animation");
	m_ScreenEffect[(int)ScreenEffect::SlashLoop] = CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_effect_screenattack_slashloop_dodge"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_effect_screenattack_slashloop_dodge.animation");
	m_ScreenEffect[(int)ScreenEffect::SlashEnd] = CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_effect_screenattack_slashend_dodge"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_effect_screenattack_slashend_dodge.animation");
	m_ScreenEffect[(int)ScreenEffect::SignalLoop] = CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_effect_screenattack_signalloop_dodge"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_effect_screenattack_signalloop_dodge.animation");
	m_ScreenEffect[(int)ScreenEffect::Crash] = CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_effect_screenattack_maincrash_dodge"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_effect_screenattack_maincrash_dodge.animation");
}

CAoE::~CAoE()
{
	delete m_RingSlash;
	for (int i = 0; i < (int)ScreenEffect::END; ++i)
	{
		delete m_ScreenEffect[i];
	}

	for (int i = 0; i < (int)RiseExplosion::END; ++i)
	{
		for (int j = 0; j < m_RiseExplosion[i].size(); ++j)
		{
			delete m_RiseExplosion[i][j];
		}
	}
}

void CAoE::Begin()
{

}

void CAoE::Tick()
{
	m_Timer += CTimeMgr::GetInst()->GetDeltaTime();
	if (m_CastTime > m_Timer)
	{
		if (m_Type == AoEType::ScreenAtk)
		{
			m_ScreenEffect[(int)ScreenEffect::SlashStart]->FinalTick();
			m_ScreenEffect[(int)ScreenEffect::SlashLoop]->FinalTick();
			m_ScreenEffect[(int)ScreenEffect::SignalLoop]->FinalTick();
		}
	}
	else
	{
		if (m_Type == AoEType::Donut)
		{
			if (!m_IsCastFin)
			{
				m_IsCastFin = true;
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(1, 7);
				int num = dis(gen);
				CSoundMgr::GetInst()->GetSound(L"largo_4th_screen_thin_slash_0" + std::to_wstring(num)
					, L"\\sound\\sounds_mon_duskyisland\\largo_4th_screen_thin_slash_0" + std::to_wstring(num) + L".ogg")->Play();
			}
			m_RingSlash->FinalTick();
			if (m_RingSlash->GetCurSceneNum() == m_RingSlash->GetFinal())
				m_IsEffectFin = true;
		}
		else if (m_Type == AoEType::ScreenAtk)
		{
			if (!m_IsCastFin)
			{
				m_IsCastFin = true;
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(1, 5);
				int num = dis(gen);
				CSoundMgr::GetInst()->GetSound(L"largo_4th_screen_slash_crash_0" + std::to_wstring(num)
					, L"\\sound\\sounds_mon_duskyisland\\largo_4th_screen_slash_crash_0" + std::to_wstring(num) + L".ogg")->Play();
			}
			m_ScreenEffect[(int)ScreenEffect::SlashEnd]->FinalTick();
			m_ScreenEffect[(int)ScreenEffect::Crash]->FinalTick();
			if (m_ScreenEffect[(int)ScreenEffect::SlashEnd]->GetCurSceneNum() == m_ScreenEffect[(int)ScreenEffect::SlashEnd]->GetFinal() 
				&& m_ScreenEffect[(int)ScreenEffect::Crash]->GetCurSceneNum() == m_ScreenEffect[(int)ScreenEffect::Crash]->GetFinal())
				m_IsEffectFin = true;
		}
		else if (m_Type == AoEType::ScreenSlash)
		{
			if (!m_IsCastFin)
			{
				m_IsCastFin = true;
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(1, 7);
				int num = dis(gen);
				CSoundMgr::GetInst()->GetSound(L"largo_4th_screen_thin_slash_0" + std::to_wstring(num)
					, L"\\sound\\sounds_mon_duskyisland\\largo_4th_screen_thin_slash_0" + std::to_wstring(num) + L".ogg")->Play();
			}
			m_ScreenEffect[(int)ScreenEffect::SlashEnd]->FinalTick();
			if (m_ScreenEffect[(int)ScreenEffect::SlashEnd]->GetCurSceneNum() == m_ScreenEffect[(int)ScreenEffect::SlashEnd]->GetFinal())
				m_IsEffectFin = true;
		}
		else
		{
			m_IsCastFin = true;
			m_IsEffectFin = true;
		}
	}
}

void CAoE::Render()
{
	float progress = min(m_Timer / m_CastTime, 1.f);
	switch (m_Type)
	{
	case AoEType::Circle:
		CTextureMgr::GetInst()->FillEllipse(Color(50, 255, 0, 0), m_Pos - Vec2D(m_Param1, m_Param1 / 3), Vec2D(m_Param1 * 2, m_Param1 * 2 / 3));
		CTextureMgr::GetInst()->FillEllipse(Color(255, 255, 0, 0), m_Pos - Vec2D(m_Param1 * progress, m_Param1 * progress / 3)
			, Vec2D(m_Param1 * progress * 2, m_Param1 * progress * 2 / 3));
		break;
	case AoEType::Donut:
		if (!m_IsCastFin)
		{
			CTextureMgr::GetInst()->FillDonut(Color(50, 255, 0, 0), m_Pos - Vec2D(m_Param1, m_Param1 / 3), Vec2D(m_Param1 * 2, m_Param1 * 2 / 3)
				, m_Pos - Vec2D(m_Param2, m_Param2 / 3), Vec2D(m_Param2 * 2, m_Param2 * 2 / 3));
			CTextureMgr::GetInst()->FillDonut(Color(255, 255, 0, 0)
				, m_Pos - Vec2D((m_Param1 - m_Param2) * progress + m_Param2, ((m_Param1 - m_Param2) * progress + m_Param2) / 3)
				, Vec2D(((m_Param1 - m_Param2) * progress + m_Param2) * 2, ((m_Param1 - m_Param2) * progress + m_Param2) * 2 / 3)
				, m_Pos - Vec2D(m_Param2, m_Param2 / 3)
				, Vec2D(m_Param2 * 2, m_Param2 * 2 / 3));
		}
		else
		{
			m_RingSlash->Render(this);
		}
		break;
	case AoEType::Rect:
		if (!m_IsCastFin)
		{
			CTextureMgr::GetInst()->FillRect(Color(50, 255, 0, 0), m_Pos - Vec2D(m_Param1 / 2, m_Param2 / 2), Vec2D(m_Param1, m_Param2));
			if (m_IsVert)
			{
				CTextureMgr::GetInst()->FillRect(Color(255, 255, 0, 0), m_Pos - Vec2D(m_Param1 * progress / 2.f, m_Param2 / 2.f), Vec2D(m_Param1 * progress, (float)m_Param2));
			}
			else
			{
				CTextureMgr::GetInst()->FillRect(Color(255, 255, 0, 0), m_Pos - Vec2D(m_Param1 / 2.f, m_Param2 * progress / 2.f), Vec2D((float)m_Param1, m_Param2 * progress));
			}
		}
		else
		{
			for (int i = 0; i < m_RiseExplosion[(int)RiseExplosion::Bottom].size(); ++i)
			{
				m_RiseExplosion[(int)RiseExplosion::Bottom][i]->Render(this);
			}
			for (int i = 0; i < m_RiseExplosion[(int)RiseExplosion::Top].size(); ++i)
			{
				m_RiseExplosion[(int)RiseExplosion::Top][i]->Render(this);
			}
		}
		break;
	case AoEType::ScreenAtk:
		if (!m_bInit)
		{
			// 각도 초기화
			for (int i = 0; i < (int)ScreenEffect::END; ++i)
			{
				m_ScreenEffect[i]->SetAngle((float)m_Param1);
			}
			m_bInit = true;
		}
		if (!m_IsCastFin)
		{
			m_ScreenEffect[(int)ScreenEffect::SlashStart]->Render(this);
			m_ScreenEffect[(int)ScreenEffect::SlashLoop]->Render(this);
			m_ScreenEffect[(int)ScreenEffect::SignalLoop]->Render(this);
			CTextureMgr::GetInst()->DrawLine(Color(255, 0, 0, 0), 12, 1300, m_Pos, (float)m_Param1);
			CTextureMgr::GetInst()->FillDividedLine(Color(100, 255, 216, 2), m_Pos, (float)m_Param1);
		}
		else
		{
			m_ScreenEffect[(int)ScreenEffect::SlashEnd]->Render(this);
			m_ScreenEffect[(int)ScreenEffect::Crash]->Render(this);
		}
		break;
	case AoEType::ScreenSlash:
		if (!m_bInit)
		{
			// 각도 초기화
			for (int i = 0; i < (int)ScreenEffect::END; ++i)
			{
				m_ScreenEffect[i]->SetAngle((float)m_Param1);
			}
			m_bInit = true;
		}
		if (!m_IsCastFin)
		{
			m_ScreenEffect[(int)ScreenEffect::SlashStart]->Render(this);
			m_ScreenEffect[(int)ScreenEffect::SlashLoop]->Render(this);
			CTextureMgr::GetInst()->DrawLine(Color(255, 0, 0, 0), 12 - 12 * progress, 1300, m_Pos, (float)m_Param1);
		}
		else
		{
			m_ScreenEffect[(int)ScreenEffect::SlashEnd]->Render(this);
		}
		break;
	}
}

void CAoE::SetInfo(AoEType _type, Vec2D _pos, int _param1, int _param2, float _castTime, bool _isVert)
{
	m_Type = _type;
	m_Pos = _pos;
	m_Param1 = _param1;
	m_Param2 = _param2;
	m_CastTime = _castTime;
	m_IsVert = _isVert;

	SetLocation(_pos);

}
