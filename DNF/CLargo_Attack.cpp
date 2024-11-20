#include "pch.h"
#include "CLargo_Attack.h"
#include "CTimeMgr.h"
#include "CObj.h"
#include "CAlbumPlayer.h"
#include "CLevelMgr.h"
#include "CPlayer.h"
#include "CEngine.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CS03.h"
#include "CPlayer.h"
#include "CRigidBody.h"

CLargo_Attack::CLargo_Attack()
	: CState(L"Largo_Attack")
	, m_SkillDuration(0.f)
	, m_CurrentSkill(LargoSkill::END)
	, m_Playlist{}
	, m_Playlist1{}
	, m_PlaylistIdx(0)
	, m_IsOpening(false)
	, m_IdleAnimation(nullptr)
	, m_IdleAnimation1(nullptr)
	, m_Player(nullptr)
	, m_Flag{false}
{
	m_IdleAnimation = CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Idle_0"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Idle_0.animation");
	m_IdleAnimation1 = CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Idle_1"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Idle_1.animation");
}

CLargo_Attack::~CLargo_Attack()
{
	ClearPlaylist();
	delete m_IdleAnimation;
	delete m_IdleAnimation1;
}

void CLargo_Attack::Enter()
{
	GetOwnerObj()->SetState((int)LargoState::Attack);
	if (GetFSM()->GetPrevState() == GetFSM()->FindState((int)LargoState::Income))
	{
		m_CurrentSkill = LargoSkill::Opening;
		m_IsOpening = true;
	}
	m_Player = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();
}

void CLargo_Attack::FinalTick()
{
	if (GetOwnerObj()->GetCurHP() <= 0)
	{
		GetFSM()->ChangeState((int)LargoState::Death);
	}

	if (CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetYogiGauge() > 100)
	{
		GetFSM()->ChangeState((int)LargoState::Pattern);
	}

	std::random_device rd;
	std::mt19937 gen(rd());

	if (!m_Playlist.empty())
	{
		m_Playlist[m_PlaylistIdx]->FinalTick();
		m_Playlist1[m_PlaylistIdx]->FinalTick();
	}
	else
	{
		m_IdleAnimation->FinalTick();
		m_IdleAnimation1->FinalTick();
	}

	m_SkillDuration -= CTimeMgr::GetInst()->GetDeltaTime();
	CS03* pLevel = (CS03*)CLevelMgr::GetInst()->GetCurrentLevel();
	if (m_SkillDuration < 0)
	{
		m_PlaylistIdx = 0;
		std::memset(m_Flag, 0, sizeof(m_Flag));
		m_Coord = Vec2D(0, 0);

		// 스킬이 시작될 때 라르고 바라보는 방향 변경
		GetOwnerObj()->SetLookLeft(CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetGroundPos().x < GetOwnerObj()->GetGroundPos().x);

		if (m_CurrentSkill == LargoSkill::Opening && m_IsOpening)
		{
			std::uniform_int_distribution<> disX(-200, 200);
			std::uniform_int_distribution<> disY(-200, 200);

			ClearPlaylist();
			AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Roar_0"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Roar_0.animation"));
			AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Slash0_0"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Slash0_0.animation"));

			AddPlaylist1(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Roar_1"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Roar_1.animation"));
			AddPlaylist1(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Slash0_1"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Slash0_1.animation"));
			pLevel->AddAoE(AoEType::Donut, GetOwnerObj()->GetGroundPos(), 280, 250, 3.f);
			for (int i = 0; i < 2; ++i)
			{
				pLevel->AddAoE(AoEType::Donut, GetOwnerObj()->GetGroundPos() + Vec2D(disX(gen) - 250 + (500 * i), disY(gen)), 280, 250, 3.f);
			}
			m_SkillDuration = 10.f;
			CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_meet_02"
				, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_meet_02.ogg")->Play();
			m_IsOpening = false;
		}
		else
		{
			// 사용 스킬을 랜덤으로 선정
			std::uniform_int_distribution<> dis(1, (int)LargoSkill::END - 1);
			int SkillNum = dis(gen);
			while ((int)m_CurrentSkill == SkillNum)
				SkillNum = dis(gen);

			switch ((LargoSkill)SkillNum)
			{
			case LargoSkill::Diagonal_Slash:
			{
				m_CurrentSkill = LargoSkill::Diagonal_Slash;

				ClearPlaylist();
				AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Slash0_0"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Slash0_0.animation"));
				AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Slash1_0"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Slash1_0.animation"));
				AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Slam_0"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Slam_0.animation"));

				AddPlaylist1(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Slash0_1"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Slash0_1.animation"));
				AddPlaylist1(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Slash1_1"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Slash1_1.animation"));
				AddPlaylist1(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_Slam_1"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_Slam_1.animation"));
				m_SkillDuration = 8;
				CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_skill_20"
					, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_skill_20.ogg")->Play();
			}
				break;
			case LargoSkill::Summon_Clone_Slash:
			{
				m_CurrentSkill = LargoSkill::Summon_Clone_Slash;
				std::uniform_int_distribution<> disX(484, 1084);
				std::uniform_int_distribution<> disY(500, 900);

				ClearPlaylist();
				AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_JumpReady_0"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_JumpReady_0.animation"));
				AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_JumpSlash_0"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_JumpSlash_0.animation"));

				AddPlaylist1(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_JumpReady_1"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_JumpReady_1.animation"));
				AddPlaylist1(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_JumpSlash_1"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_JumpSlash_1.animation"));
				m_SkillDuration = 10;
				CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_skill_19_3"
					, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_skill_19_3.ogg")->Play();
				m_Coord = Vec2D(disX(gen), disY(gen));
				pLevel->AddAoE(AoEType::Circle, m_Coord, 280, 0, 6.f);
			}
				break;
			case LargoSkill::Cross_Slash_1:
			{
				m_CurrentSkill = LargoSkill::Cross_Slash_1;

				std::uniform_int_distribution<> dis(1, 2);
				std::uniform_int_distribution<> disY(500, 900);

				int posX = ((GetOwnerObj()->GetGroundPos() - m_Player->GetGroundPos()).x < 0) ? 368 : 1200;

				ClearPlaylist();
				AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_BackflipSlam_0"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_BackflipSlam_0.animation"));
				AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_BackflipSlash_0"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_BackflipSlash_0.animation"));

				AddPlaylist1(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_BackflipSlam_1"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_BackflipSlam_1.animation"));
				AddPlaylist1(CAlbumPlayer::CreatePlayerFromFile(L"boss_unshackled_BackflipSlash_1"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\boss_unshackled_BackflipSlash_1.animation"));
				m_SkillDuration = 5;
				int num = dis(gen);
				CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_skill_07_" + std::to_wstring(num)
					, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_skill_07_" + std::to_wstring(num) + L".ogg")->Play();
				m_Coord = Vec2D(posX, disY(gen));
				pLevel->AddAoE(AoEType::Rect, Vec2D(600.f, m_Coord.y), 200, 1000, 1.5f, true);
				pLevel->AddAoE(AoEType::Rect, Vec2D(600.f, m_Coord.y), 1600, 300, 2.5f, false);
				Vec2D dir = m_Coord - GetOwnerObj()->GetGroundPos();
				GetOwnerObj()->GetRigidBody()->Jump(1000);
				GetOwnerObj()->GetRigidBody()->SetSpeed(dir / 2.f);
			}
				break;
			}
		}
	}
	else
	{
		// 스킬별 프레임마다 할 일
		switch (m_CurrentSkill)
		{
		case LargoSkill::Opening:
			if (!m_Playlist.empty())
			{
				if (m_Playlist[0]->GetCurSceneNum() == m_Playlist[0]->GetFinal() && m_Flag[0] == false)
				{
					std::uniform_int_distribution<> dis(1, 8);
					int num = dis(gen);
					CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_atk_0" + std::to_wstring(num)
						, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_atk_0" + std::to_wstring(num) + L".ogg")->Play();
					m_Flag[0] = true;
				}
				else if (m_Playlist[1]->GetCurSceneNum() == (m_Playlist[1]->GetFinal() / 2) && m_Flag[0] == true && m_Flag[1] == false)
				{
					std::uniform_int_distribution<> disA(0, 360);
					pLevel->AddAoE(AoEType::ScreenAtk, GetOwnerObj()->GetGroundPos(), disA(gen), 0, 2.f);
					m_Flag[1] = true;
				}
			}
			break;
		case LargoSkill::Diagonal_Slash:
			if (!m_Playlist.empty())
			{
				std::uniform_int_distribution<> disX(-100, 100);
				std::uniform_int_distribution<> disY(-50, 50);
				std::uniform_int_distribution<> disA1(0, 360);
				std::uniform_int_distribution<> disA2(-20, 20);
				std::uniform_int_distribution<> disV(1, 8);
				if (m_Playlist[0]->GetCurSceneNum() == (m_Playlist[0]->GetFinal() / 2) && m_Flag[0] == false)
				{
					int num = disV(gen);
					int angle = disA1(gen);
					CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_atk_0" + std::to_wstring(num)
						, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_atk_0" + std::to_wstring(num) + L".ogg")->Play();
					for (int i = 0; i < 2; ++i)
					{
						pLevel->AddAoE(AoEType::ScreenSlash, m_Player->GetGroundPos() + Vec2D(disX(gen), disY(gen)), angle + disA2(gen), 0, 2.f);
					}
					GetOwnerObj()->GetRigidBody()->SetSpeed((m_Player->GetGroundPos() - GetOwnerObj()->GetGroundPos()).Normalize() * 100);
					m_Flag[0] = true;
				}
				else if (m_Playlist[1]->GetCurSceneNum() == (m_Playlist[1]->GetFinal() / 2) && m_Flag[0] == true && m_Flag[1] == false)
				{
					int num = disV(gen);
					int angle = disA1(gen);
					CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_atk_0" + std::to_wstring(num)
						, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_atk_0" + std::to_wstring(num) + L".ogg")->Play();
					for (int i = 0; i < 2; ++i)
					{
						pLevel->AddAoE(AoEType::ScreenSlash, m_Player->GetGroundPos() + Vec2D(disX(gen), disY(gen)), angle + disA2(gen), 0, 2.f);
					}
					GetOwnerObj()->GetRigidBody()->SetSpeed((m_Player->GetGroundPos() - GetOwnerObj()->GetGroundPos()).Normalize() * 100);
					m_Flag[1] = true;
				}
				else if (m_Playlist[2]->GetCurSceneNum() == (m_Playlist[2]->GetFinal() / 2) && m_Flag[1] == true && m_Flag[2] == false)
				{
					int num = disV(gen);
					int angle = disA1(gen) / 2;
					float rad = angle * PI / 180;
					Vec2D pos = Vec2D(sinf(rad), -cosf(rad)) * 100;
					CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_atk_0" + std::to_wstring(num)
						, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_atk_0" + std::to_wstring(num) + L".ogg")->Play();
					for (int i = 0; i < 2; ++i)
					{
						pLevel->AddAoE(AoEType::ScreenAtk, m_Player->GetGroundPos() + pos - (pos * 2 * i), angle + (180 * i), 0, 2.f);
					}
					GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));
					m_Flag[2] = true;
				}
			}
			break;
		case LargoSkill::Summon_Clone_Slash:
			if (!m_Playlist.empty())
			{
				if (m_Playlist[0]->GetCurSceneNum() == m_Playlist[0]->GetFinal() && m_Flag[0] == false)
				{
					CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_skill_19_4"
						, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_skill_19_4.ogg")->Play();
					Vec2D dir = m_Coord - GetOwnerObj()->GetGroundPos();
					GetOwnerObj()->GetRigidBody()->Jump(1300);
					GetOwnerObj()->GetRigidBody()->SetSpeed(dir / 3.f);
					m_Flag[0] = true;
				}
				else if (m_Playlist[1]->GetCurSceneNum() == m_Playlist[1]->GetFinal() && m_Flag[0] == true && m_Flag[1] == false)
				{
					GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));
					std::uniform_int_distribution<> disA(20, 40);
					Vec2D vec1 = GetOwnerObj()->GetGroundPos() - Vec2D(784, 700);
					float angle1 = acosf(vec1.x / vec1.Length()) * 180.f / PI ;
					angle1 = (vec1.y >= 0) ? angle1 : -angle1;
					pLevel->AddAoE(AoEType::ScreenAtk, Vec2D(784, 700), angle1 - disA(gen), 0, 2.f);
					pLevel->AddAoE(AoEType::ScreenAtk, Vec2D(784, 700), angle1 + disA(gen) + 180, 0, 2.f);
					m_Flag[1] = true;
				}
			}
			break;
		case LargoSkill::Cross_Slash_1:
			if (!m_Playlist.empty())
			{
				if (m_Playlist[0]->GetCurSceneNum() == m_Playlist[0]->GetFinal() && m_Flag[0] == false)
				{
					std::uniform_int_distribution<> disV(1, 8);
					int num = disV(gen);
					CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_atk_0" + std::to_wstring(num)
						, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_atk_0" + std::to_wstring(num) + L".ogg")->Play();
					GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));
					m_Flag[0] = true;
				}
			}
			break;
		}
	}
}

void CLargo_Attack::Exit()
{
}

void CLargo_Attack::Render()
{
	// 플레이리스트가 비었으면 기본 대기모션 재생, 있으면 플레이리스트 순차재생
	if (m_Playlist.size() == 0)
	{
		m_IdleAnimation->Render(GetOwnerObj(), false, GetOwnerObj()->IsLookLeft());
		m_IdleAnimation1->Render(GetOwnerObj(), false, GetOwnerObj()->IsLookLeft());
	}
	else
	{
		m_Playlist[m_PlaylistIdx]->Render(GetOwnerObj(), false, GetOwnerObj()->IsLookLeft());
		m_Playlist1[m_PlaylistIdx]->Render(GetOwnerObj(), false, GetOwnerObj()->IsLookLeft());
		if (m_Playlist[m_PlaylistIdx]->GetCurSceneNum() == m_Playlist[m_PlaylistIdx]->GetFinal())
		{
			if (m_PlaylistIdx < m_Playlist.size() - 1)
			{
				++m_PlaylistIdx;
			}
			else if (m_PlaylistIdx == m_Playlist.size() - 1)
			{
				ClearPlaylist();
			}
		}
	}
}

void CLargo_Attack::ClearPlaylist()
{
	for (int i = 0; i < m_Playlist.size(); ++i)
	{
		if (m_Playlist[i] != nullptr)
		{
			delete m_Playlist[i];
		}
	}
	m_Playlist.clear();

	for (int i = 0; i < m_Playlist1.size(); ++i)
	{
		if (m_Playlist1[i] != nullptr)
		{
			delete m_Playlist1[i];
		}
	}
	m_Playlist1.clear();
}