#include "pch.h"
#include "CAoE.h"
#include "CTimeMgr.h"
#include "CTextureMgr.h"
#include "CLevelMgr.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CPlayer.h"
#include "CCollider.h"

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
	m_Player = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();
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
		// 시전 중
		if (m_Type == AoEType::ScreenAtk)
		{
			m_ScreenEffect[(int)ScreenEffect::SlashStart]->FinalTick();
			m_ScreenEffect[(int)ScreenEffect::SlashLoop]->FinalTick();
			m_ScreenEffect[(int)ScreenEffect::SignalLoop]->FinalTick();
		}
	}
	else
	{
		// 시전 종료 이후
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
				float dist = (m_Pos - m_Player->GetGroundPos()).Length();
				if (dist < m_Param1 && dist > m_Param2)
					m_Player->GiveDamage(m_Player->GetMaxHP() * 0.1f);
			}
			m_RingSlash->FinalTick();
			if (m_RingSlash->GetCurSceneNum() == m_RingSlash->GetFinal())
				m_IsEffectFin = true;
		}
		else if (m_Type == AoEType::Circle)
		{
			if (!m_IsCastFin)
			{
				m_IsCastFin = true;
				float dist = (m_Pos - m_Player->GetGroundPos()).Length();
				if (dist < m_Param1)
					m_Player->GiveDamage(m_Player->GetMaxHP() * 0.1f);
			}
		}
		else if (m_Type == AoEType::Rect)
		{
			if (!m_IsCastFin)
			{
				m_IsCastFin = true;
				Vec2D LT = m_Pos - Vec2D(m_Param1 / 2, m_Param2 / 2);
				Vec2D RB = m_Pos + Vec2D(m_Param1 / 2, m_Param2 / 2);
				if (m_Player->GetGroundPos() >> LT && m_Player->GetGroundPos() << RB)
					m_Player->GiveDamage(m_Player->GetMaxHP() * 0.3f);
			}
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
				if (LineCrossCheck(m_Player->GetBodyCollider()->GetLocation(), m_Player->GetBodyCollider()->GetLocation() + m_Player->GetBodyCollider()->GetSize()
					, 1300, m_Pos, (float)m_Param1) != -1)
					m_Player->GiveDamage(m_Player->GetMaxHP() * 1.1f);
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
				if (LineCrossCheck(m_Player->GetBodyCollider()->GetLocation(), m_Player->GetBodyCollider()->GetLocation() + m_Player->GetBodyCollider()->GetSize()
					, 1300, m_Pos, (float)m_Param1) == 0)
					m_Player->GiveDamage(m_Player->GetMaxHP() * 0.2f);
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
		if (!m_IsCastFin)
		{
			CTextureMgr::GetInst()->FillEllipse(Color(50, 255, 0, 0), m_Pos - Vec2D(m_Param1, m_Param1 / 3), Vec2D(m_Param1 * 2, m_Param1 * 2 / 3));
			CTextureMgr::GetInst()->FillEllipse(Color(255, 255, 0, 0), m_Pos - Vec2D(m_Param1 * progress, m_Param1 * progress / 3)
				, Vec2D(m_Param1 * progress * 2, m_Param1 * progress * 2 / 3));
		}
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

int CAoE::LineCrossCheck(Vec2D _LT, Vec2D _RB, int _Len, Vec2D _LinePos, float _LineAngle)
{
	float rad = _LineAngle * PI / 180.f;
	Vec2D DirVec(cosf(rad), sinf(rad));
	Vec2D _begin = _LinePos + (DirVec * _Len / 2);
	Vec2D _end = _LinePos - (DirVec * _Len / 2);
	Vec2D linevec = _end - _begin;
	Vec2D ref[4] =
	{
		_LT - _begin,
		Vec2D(_RB.x, _LT.y) - _begin,
		_RB - _begin,
		Vec2D(_LT.x, _RB.y) - _begin
	};

	if (linevec.Cross(ref[0]) > 0 && linevec.Cross(ref[1]) > 0 && linevec.Cross(ref[2]) > 0 && linevec.Cross(ref[3]) > 0)
		return 1;

	if (linevec.Cross(ref[0]) < 0 && linevec.Cross(ref[1]) < 0 && linevec.Cross(ref[2]) < 0 && linevec.Cross(ref[3]) < 0)
		return -1;

	return 0;
}
