#include "pch.h"
#include "CPlayer_Attack.h"
#include "CObj.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CAlbumPlayer.h"
#include "CTimeMgr.h"
#include "CEngine.h"
#include "CPlayer.h"
#include "CArrow.h"
#include "CLevelMgr.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CPlayerSkill.h"
#include "CPlayerAoE.h"
#include "CDummy.h"
#include "CCameraMgr.h"

CPlayer_Attack::CPlayer_Attack(wstring _name)
	: CState(_name)
	, m_SkillDuration(0.f)
	, m_PlaylistTimer(0.f)
	, m_PlaylistEffTimer{}
	, m_PlaylistScrTimer{}
	, m_IdleTimer(0.f)
	, m_Cooltime{}
	, m_CurrentSkill(PlayerSkill::END)
	, m_TappedKey(Keyboard::Keyboard_end)
	, m_Playlist{}
	, m_PlaylistAvt{}
	, m_PlaylistEff{}
	, m_PlaylistScr{}
	, m_PlaylistIdx(0)
	, m_PlaylistEffIdx{}
	, m_PlaylistScrIdx{}
	, m_IdleAnimation(nullptr)
	, m_IdleAnimationAvt(25, nullptr)
	, m_Screen(nullptr)
{
	m_IdleAnimation = CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Attack_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Attack_Idle.animation");
	m_Screen = new CDummy(L"Screen");
	m_Screen->SetScale(Vec2D(1066, 600));
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_Screen, LayerType::Far);
}

CPlayer_Attack::~CPlayer_Attack()
{
	ClearAllPlaylist();
	delete m_IdleAnimation;
	for (int i = 0; i < 25; ++i)
		delete m_IdleAnimationAvt[i];
	//delete m_Screen;
}

void CPlayer_Attack::Enter()
{
	m_IdleAnimation->SetCurrentScene(0);
	CAlbumPlayer::CreateAvatar(L"archer_skin_00_Attack_Idle", ((CPlayer*)GetOwnerObj())->GetCurAvatarCode(), m_IdleAnimationAvt);
	GetOwnerObj()->SetState((int)PlayerState::Attack);
	GetOwnerObj()->GetBodyCollider()->SetSize(Vec2D(64, 82));
	GetOwnerObj()->GetBodyCollider()->SetOffset(Vec2D(0, 15));
	GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));
}

void CPlayer_Attack::FinalTick()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	if (m_SkillDuration < 0)
	{
		// 스킬 지속시간이 끝난 경우, 입력 대기상태
		
		std::memset(m_Flag, 0, sizeof(m_Flag));
		m_Coord = Vec2D(0, 0);

		CPlayer* pPlayer = (CPlayer*)GetOwnerObj();
		if (m_TappedKey == Keyboard::Z)
		{
			// 지상에서만 사용 가능
			if (GetOwnerObj()->GetRigidBody()->GetAirborne() == false)
			{
				// 일반공격 (3단)
				if (m_CurrentSkill == PlayerSkill::Basic0 && m_PlaylistIdx > -1.f)
				{
					ClearAllPlaylist();
					m_CurrentSkill = PlayerSkill::Basic1;
					m_SkillDuration = 0.2f;
					AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Shoot_1", CEngine::GetInst()->GetResourcePathW()
						+ L"\\animation\\archer_skin_00_Shoot_1.animation"));
					AddPlaylistAvt(CAlbumPlayer::CreateAvatarArr(L"archer_skin_00_Shoot_1", pPlayer->GetCurAvatarCode()));
					AddPlaylistEff(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow_shootA_0", CEngine::GetInst()->GetResourcePathW()
						+ L"\\animation\\archer_effect_arrow_shootA_0.animation", Vec2D(40, 30)), EffectLayer::a);
					AddPlaylistEff(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow_shootA_1", CEngine::GetInst()->GetResourcePathW()
						+ L"\\animation\\archer_effect_arrow_shootA_1.animation", Vec2D(40, 30)), EffectLayer::b);
					CArrow* pArrow = new CArrow(L"Basic0_Arrow", Vec2D(GetOwnerObj()->IsLookLeft() ? -1000 : 1000, 0), -50, 3);
					pArrow->SetLocation(GetOwnerObj()->GetLocation() + GetOwnerObj()->GetScale() / 2.f);
					pArrow->GetRigidBody()->SetGravityReduce(900);
					pArrow->SetIdleAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow"
						, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_arrow.animation"));
					pArrow->SetDeadAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_hiteffect"
						, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_hiteffect.animation"), ArrowEffLayer::a);
					pArrow->SetDeadAniG(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow_groundhit"
						, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_arrow_groundhit.animation", Vec2D(0, -15)), ArrowEffLayer::a);
					pArrow->SetATK(100);
					CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pArrow, LayerType::Object);
					std::uniform_int_distribution<> dis(1, 9);
					CSoundMgr::GetInst()->GetSound(L"lbow_01_" + std::to_wstring(dis(gen))
						, L"\\sound\\sounds_char_archer_weapon\\lbow_01_" + std::to_wstring(dis(gen)) + L".ogg")->Play();
				}
				else if (m_CurrentSkill == PlayerSkill::Basic1 && m_PlaylistIdx > -1.f)
				{
					ClearAllPlaylist();
					m_CurrentSkill = PlayerSkill::Basic2;
					m_SkillDuration = 0.5f;
					AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Shoot_2", CEngine::GetInst()->GetResourcePathW()
						+ L"\\animation\\archer_skin_00_Shoot_2.animation"));
					AddPlaylistAvt(CAlbumPlayer::CreateAvatarArr(L"archer_skin_00_Shoot_2", pPlayer->GetCurAvatarCode()));
					AddPlaylistEff(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow_shootB_0", CEngine::GetInst()->GetResourcePathW()
						+ L"\\animation\\archer_effect_arrow_shootB_0.animation", Vec2D(40, 30)), EffectLayer::a);
					AddPlaylistEff(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow_shootB_1", CEngine::GetInst()->GetResourcePathW()
						+ L"\\animation\\archer_effect_arrow_shootB_1.animation", Vec2D(40, 30)), EffectLayer::b);
					CArrow* pArrow = new CArrow(L"Basic0_Arrow", Vec2D(GetOwnerObj()->IsLookLeft() ? -1000 : 1000, 0), -50, 3);
					pArrow->SetLocation(GetOwnerObj()->GetLocation() + GetOwnerObj()->GetScale() / 2.f);
					pArrow->GetRigidBody()->SetZeroGravity(true);
					pArrow->SetIdleAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow"
						, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_arrow.animation"));
					pArrow->SetDeadAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_hiteffect"
						, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_hiteffect.animation"), ArrowEffLayer::a);
					pArrow->SetDeadAniG(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow_groundhit"
						, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_arrow_groundhit.animation", Vec2D(0, -15)), ArrowEffLayer::a);
					pArrow->SetATK(200);
					CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pArrow, LayerType::Object);
					std::uniform_int_distribution<> dis(1, 9);
					CSoundMgr::GetInst()->GetSound(L"lbow_01_" + std::to_wstring(dis(gen))
						, L"\\sound\\sounds_char_archer_weapon\\lbow_01_" + std::to_wstring(dis(gen)) + L".ogg")->Play();
				}
				else
				{
					ClearAllPlaylist();
					m_CurrentSkill = PlayerSkill::Basic0;
					m_SkillDuration = 0.4f;
					AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Shoot_0", CEngine::GetInst()->GetResourcePathW()
						+ L"\\animation\\archer_skin_00_Shoot_0.animation"));
					AddPlaylistAvt(CAlbumPlayer::CreateAvatarArr(L"archer_skin_00_Shoot_0", pPlayer->GetCurAvatarCode()));
					AddPlaylistEff(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow_shootA_0", CEngine::GetInst()->GetResourcePathW()
						+ L"\\animation\\archer_effect_arrow_shootA_0.animation", Vec2D(40, 30)), EffectLayer::a);
					AddPlaylistEff(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow_shootA_1", CEngine::GetInst()->GetResourcePathW()
						+ L"\\animation\\archer_effect_arrow_shootA_1.animation", Vec2D(40, 30)), EffectLayer::b);
					CArrow* pArrow = new CArrow(L"Basic0_Arrow", Vec2D(GetOwnerObj()->IsLookLeft() ? -1000 : 1000, 0), -50, 3);
					pArrow->SetLocation(GetOwnerObj()->GetLocation() + GetOwnerObj()->GetScale() / 2.f);
					pArrow->GetRigidBody()->SetGravityReduce(900);
					pArrow->SetIdleAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow"
						, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_arrow.animation"));
					pArrow->SetDeadAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_hiteffect"
						, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_hiteffect.animation"), ArrowEffLayer::a);
					pArrow->SetDeadAniG(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_arrow_groundhit"
						, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_arrow_groundhit.animation", Vec2D(0, -15)), ArrowEffLayer::a);
					pArrow->SetATK(100);
					CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pArrow, LayerType::Object);
					std::uniform_int_distribution<> dis(1, 9);
					CSoundMgr::GetInst()->GetSound(L"lbow_01_" + std::to_wstring(dis(gen))
						, L"\\sound\\sounds_char_archer_weapon\\lbow_01_" + std::to_wstring(dis(gen)) + L".ogg")->Play();
				}
			}
		}
		else if (m_TappedKey == Keyboard::Q && m_Cooltime[(int)PlayerSkill::Geardrive] < 0)
		{
			ClearAllPlaylist();
			m_CurrentSkill = PlayerSkill::Geardrive;
			m_SkillDuration = 0.5f;
			m_Cooltime[(int)PlayerSkill::Geardrive] = 5.f;
			GetOwnerObj()->UseMana(100);
			// 지상 공중 사용 분기
			if (GetOwnerObj()->GetRigidBody()->GetAirborne() == false)
			{
				AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Shoot_2", CEngine::GetInst()->GetResourcePathW()
					+ L"\\animation\\archer_skin_00_Shoot_2.animation"));
				AddPlaylistAvt(CAlbumPlayer::CreateAvatarArr(L"archer_skin_00_Shoot_2", pPlayer->GetCurAvatarCode()));
				AddPlaylistEff(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_shock", CEngine::GetInst()->GetResourcePathW()
					+ L"\\animation\\archer_effect_geardrive_shock.animation", Vec2D(40, 30)), EffectLayer::a);
				CArrow* pArrow = new CArrow(L"Geardrive_Arrow", Vec2D(GetOwnerObj()->IsLookLeft() ? -1300 : 1300, 0), -50, 3);
				pArrow->SetLocation(GetOwnerObj()->GetLocation() + GetOwnerObj()->GetScale() / 2.f);
				pArrow->GetRigidBody()->SetZeroGravity(true);
				pArrow->SetIdleAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_flamearrow"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_geardrive_flamearrow.animation"));
				pArrow->SetDeadAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_explosion_0"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_geardrive_explosion_0.animation"), ArrowEffLayer::a);
				pArrow->SetDeadAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_explosion_1"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_geardrive_explosion_1.animation"), ArrowEffLayer::b);
				pArrow->SetDeadAniG(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_explosionG_0"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_geardrive_explosionG_0.animation", Vec2D(0, -15)), ArrowEffLayer::a);
				pArrow->SetDeadAniG(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_explosionG_1"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_geardrive_explosionG_1.animation", Vec2D(0, -15)), ArrowEffLayer::b);
				pArrow->SetDeadSound(CSoundMgr::GetInst()->GetSound(L"gear_drive_impact_01", L"\\sound\\sounds_char_archer_effect\\gear_drive_impact_01.ogg"));
				pArrow->SetDeadFunc(this, (DELEGATE_1)&CPlayer_Attack::GeardriveCallback);
				CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pArrow, LayerType::Object);
				std::uniform_int_distribution<> dis(1, 9);
				CSoundMgr::GetInst()->GetSound(L"lbow_01_" + std::to_wstring(dis(gen))
					, L"\\sound\\sounds_char_archer_weapon\\lbow_01_" + std::to_wstring(dis(gen)) + L".ogg")->Play();
				CSoundMgr::GetInst()->GetSound(L"ar_gear_drive_01_b", L"\\sound\\sounds_char_archer\\ar_gear_drive_01_b.ogg")->Play();
			}
			else
			{
				AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_ShootA_2", CEngine::GetInst()->GetResourcePathW()
					+ L"\\animation\\archer_skin_00_ShootA_2.animation"));
				AddPlaylistAvt(CAlbumPlayer::CreateAvatarArr(L"archer_skin_00_ShootA_2", pPlayer->GetCurAvatarCode()));
				GetOwnerObj()->GetRigidBody()->Jump(500);
				CAlbumPlayer* pAP = CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_shock", CEngine::GetInst()->GetResourcePathW()
					+ L"\\animation\\archer_effect_geardrive_shock.animation", Vec2D(40, 30));
				pAP->SetAngle(40);
				AddPlaylistEff(pAP, EffectLayer::a);
				CArrow* pArrow = new CArrow(L"Geardrive_Arrow", Vec2D(GetOwnerObj()->IsLookLeft() ? -1300 : 1300, 0)
					, -50 + GetOwnerObj()->GetRigidBody()->GetAirborneHeight(), 3);
				pArrow->GetRigidBody()->SetAirborneSpeed(1300 * tanf(40 * PI / 180));
				pArrow->SetLocation(GetOwnerObj()->GetLocation() + GetOwnerObj()->GetScale() / 2.f);
				pArrow->GetRigidBody()->SetZeroGravity(true);
				pArrow->SetIdleAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_flamearrow"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_geardrive_flamearrow.animation"));
				pArrow->SetDeadAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_explosion_0"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_geardrive_explosion_0.animation"), ArrowEffLayer::a);
				pArrow->SetDeadAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_explosion_1"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_geardrive_explosion_1.animation"), ArrowEffLayer::b);
				pArrow->SetDeadAniG(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_explosionG_0"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_geardrive_explosionG_0.animation", Vec2D(0, -15)), ArrowEffLayer::a);
				pArrow->SetDeadAniG(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_geardrive_explosionG_1"
					, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_geardrive_explosionG_1.animation", Vec2D(0, -15)), ArrowEffLayer::b);
				pArrow->SetDeadSound(CSoundMgr::GetInst()->GetSound(L"gear_drive_impact_01", L"\\sound\\sounds_char_archer_effect\\gear_drive_impact_01.ogg"));
				pArrow->SetDeadFunc(this, (DELEGATE_1)&CPlayer_Attack::GeardriveCallback);
				pArrow->SetPierceCount(100);
				CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pArrow, LayerType::Object);
				std::uniform_int_distribution<> dis(1, 9);
				CSoundMgr::GetInst()->GetSound(L"lbow_01_" + std::to_wstring(dis(gen))
					, L"\\sound\\sounds_char_archer_weapon\\lbow_01_" + std::to_wstring(dis(gen)) + L".ogg")->Play();
				CSoundMgr::GetInst()->GetSound(L"ar_gear_drive_01_a", L"\\sound\\sounds_char_archer\\ar_gear_drive_01_a.ogg")->Play();
			}
		}
		else if (m_TappedKey == Keyboard::W && m_Cooltime[(int)PlayerSkill::Mistheavyrain] < 0)
		{
			if (GetOwnerObj()->GetRigidBody()->GetAirborne() == false)
			{
				ClearAllPlaylist();
				m_CurrentSkill = PlayerSkill::Mistheavyrain;
				m_SkillDuration = 0.5f;
				m_Cooltime[(int)PlayerSkill::Mistheavyrain] = 5.f;
				GetOwnerObj()->UseMana(200);

				AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Shoot_3", CEngine::GetInst()->GetResourcePathW()
					+ L"\\animation\\archer_skin_00_Shoot_3.animation"));
				AddPlaylistAvt(CAlbumPlayer::CreateAvatarArr(L"archer_skin_00_Shoot_3", pPlayer->GetCurAvatarCode()));
				CAlbumPlayer* pAP0 = CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_mistheavyrain_wave_0", CEngine::GetInst()->GetResourcePathW()
					+ L"\\animation\\archer_effect_mistheavyrain_wave_0.animation", Vec2D(40, 30));
				pAP0->SetAngle(-40);
				AddPlaylistEff(pAP0, EffectLayer::a);
				CAlbumPlayer* pAP1 = CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_mistheavyrain_wave_1", CEngine::GetInst()->GetResourcePathW()
					+ L"\\animation\\archer_effect_mistheavyrain_wave_1.animation", Vec2D(40, 30));
				pAP1->SetAngle(-40);
				AddPlaylistEff(pAP1, EffectLayer::b);
				CAlbumPlayer* pAP2 = CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_mistheavyrain_shoot", CEngine::GetInst()->GetResourcePathW()
					+ L"\\animation\\archer_effect_mistheavyrain_shoot.animation", Vec2D(40, 30));
				pAP2->SetAngle(-40);
				AddPlaylistEff(pAP2, EffectLayer::c);
				for (int i = 0; i < 12; ++i)
				{
					MHRArrowCyan(i);
				}
				CSoundMgr::GetInst()->GetSound(L"mist_heavy_rain_shoot_01", L"\\sound\\sounds_char_archer_effect\\mist_heavy_rain_shoot_01.ogg")->Play();
				std::uniform_int_distribution<> dis1(1, 2);
				CSoundMgr::GetInst()->GetSound(L"ar_mist_heavy_rain_0" + std::to_wstring(dis1(gen))
					, L"\\sound\\sounds_char_archer\\ar_mist_heavy_rain_0" + std::to_wstring(dis1(gen)) + L".ogg")->Play();
			}
		}
		else if ((m_TappedKey == Keyboard::NUM_5 || CKeyMgr::GetInst()->GetCommand() == vector<Keyboard>{ Keyboard::LEFT, Keyboard::RIGHT, Keyboard::LCTRL }) && m_Cooltime[(int)PlayerSkill::Sagittarius] < 0)
		{
			ClearAllPlaylist();
			m_CurrentSkill = PlayerSkill::Sagittarius;
			m_SkillDuration = 7.f;
			m_Cooltime[(int)PlayerSkill::Sagittarius] = 15.f;
			GetOwnerObj()->UseMana(500);
			GetOwnerObj()->SetImmune(true);
			if (GetOwnerObj()->GetRigidBody()->GetAirborne() == false)
				GetOwnerObj()->GetRigidBody()->Jump(700);
			AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_sagittarius_jump", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\archer_skin_00_sagittarius_jump.animation"));
			AddPlaylistAvt(CAlbumPlayer::CreateAvatarArr(L"archer_skin_00_sagittarius_jump", pPlayer->GetCurAvatarCode()));
			AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_sagittarius_0", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\archer_skin_00_sagittarius_0.animation"));
			AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_sagittarius_1", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\archer_skin_00_sagittarius_1.animation"));
			AddPlaylist(CAlbumPlayer::CreatePlayerFromFile(L"dummy_empty", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\dummy_empty.animation"), 2.f);
			AddPlaylistScr(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_sagittarius_bg_0", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\archer_effect_sagittarius_bg_0.animation"), EffectLayer::a, m_SkillDuration);
			AddPlaylistScr(CAlbumPlayer::CreatePlayerFromFile(L"dummy_empty", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\dummy_empty.animation"), EffectLayer::b, 2.f);
			AddPlaylistScr(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_sagittarius_cutin", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\archer_effect_sagittarius_cutin.animation"), EffectLayer::b);
			AddPlaylistScr(CAlbumPlayer::CreatePlayerFromFile(L"dummy_empty", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\dummy_empty.animation"), EffectLayer::b, 1.5f);
			AddPlaylistScr(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_sagittarius_star_2", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\archer_effect_sagittarius_star_2.animation"), EffectLayer::b, 1.f);
			AddPlaylistScr(CAlbumPlayer::CreatePlayerFromFile(L"dummy_empty", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\dummy_empty.animation"), EffectLayer::c, 6.f);
			AddPlaylistScr(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_sagittarius_star_1", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\archer_effect_sagittarius_star_1.animation"), EffectLayer::c, 1.f);
			AddPlaylistScr(CAlbumPlayer::CreatePlayerFromFile(L"dummy_empty", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\dummy_empty.animation"), EffectLayer::d, 5.5f);
			AddPlaylistScr(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_sagittarius_star_0", CEngine::GetInst()->GetResourcePathW()
				+ L"\\animation\\archer_effect_sagittarius_star_0.animation"), EffectLayer::d, 1.5f);
			std::uniform_int_distribution<> dis1(L'a', L'c');
			m_SagittariusCode = (WCHAR)(dis1(gen));
			CSoundMgr::GetInst()->GetSound(L"sagittarius_mist_booster", L"\\sound\\sounds_char_archer_effect\\sagittarius_mist_booster.ogg")->Play();
			CSoundMgr::GetInst()->GetSound(L"ar_sagittarius_01_" + std::wstring(1, m_SagittariusCode)
				, L"\\sound\\sounds_char_archer\\ar_sagittarius_01_" + std::wstring(1, m_SagittariusCode) + L".ogg")->Play();
		}
	}
	else
	{
		// 스킬별 프레임마다 할 일
		if (m_CurrentSkill == PlayerSkill::Sagittarius)
		{
			if (m_Flag[0] == false && GetOwnerObj()->GetRigidBody()->GetAirborneSpeed() > 0)
			{
				m_Flag[0] = true;
				GetOwnerObj()->GetRigidBody()->SetAirborneSpeed(0.f);
				GetOwnerObj()->GetRigidBody()->SetZeroGravity(true);
				
			}
			else if (m_Flag[1] == false && m_Flag[0] == true && m_PlaylistIdx >= 2)
			{
				m_Flag[1] = true;
				CSoundMgr::GetInst()->GetSound(L"sagittarius_transition_shoot", L"\\sound\\sounds_char_archer_effect\\sagittarius_transition_shoot.ogg")->Play();
				CSoundMgr::GetInst()->GetSound(L"ar_sagittarius_02_" + std::wstring(1, m_SagittariusCode)
					, L"\\sound\\sounds_char_archer\\ar_sagittarius_02_" + std::wstring(1, m_SagittariusCode) + L".ogg")->Play();
			}
			else if (m_Flag[2] == false && m_Flag[1] == true && m_PlaylistIdx >= 2 && m_PlaylistTimer > m_Playlist[2].second - 0.3f)
			{
				m_Flag[2] = true;
				CCameraMgr::GetInst()->SetEffect(CameraEffect::Flash);
				vector<CObj*> vec = CLevelMgr::GetInst()->GetCurrentLevel()->GetObjLayer(LayerType::Object);
				CObj* pPlayer = (CObj*)CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();
				for (int i = 0; i < vec.size(); ++i)
				{
					vec[i]->GiveDamage(1000);
				}
				GetOwnerObj()->GetRigidBody()->Jump(700);
				GetOwnerObj()->GetRigidBody()->SetZeroGravity(false);
				CSoundMgr::GetInst()->GetSound(L"sagittarius_constellation_exp"
					, L"\\sound\\sounds_char_archer_effect\\sagittarius_constellation_exp.ogg")->Play();
				CSoundMgr::GetInst()->GetSound(L"ar_sagittarius_03_" + std::wstring(1, m_SagittariusCode)
					, L"\\sound\\sounds_char_archer\\ar_sagittarius_03_" + std::wstring(1, m_SagittariusCode) + L".ogg")->Play();

			}
			else if (m_Flag[3] == false && m_Flag[2] == true && m_PlaylistScrIdx[(int)EffectLayer::c] >= 1 && m_PlaylistScrTimer[(int)EffectLayer::c] > 0.5f)
			{
				m_Flag[3] = true;
				CCameraMgr::GetInst()->SetEffect(CameraEffect::Flash);
				vector<CObj*> vec = CLevelMgr::GetInst()->GetCurrentLevel()->GetObjLayer(LayerType::Object);
				CObj* pPlayer = (CObj*)CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();
				for (int i = 0; i < vec.size(); ++i)
				{
					vec[i]->GiveDamage(1500);
				}
				GetOwnerObj()->SetImmune(false);
			}
		}
	}


	if (!m_Playlist.empty())
	{
		// 플레이어가 공격 모션이 있으면
		m_IdleTimer = 0.f;
		m_Playlist[m_PlaylistIdx].first->FinalTick();
		for (int i = 0; i < 25; ++i)
		{
			if (m_PlaylistAvt.size() > m_PlaylistIdx)
			{
				if (m_PlaylistAvt[m_PlaylistIdx].first[i])
					m_PlaylistAvt[m_PlaylistIdx].first[i]->FinalTick();
			}
		}
		for (int i = 0; i < (int)EffectLayer::END; ++i)
		{
			if (!m_PlaylistEff[i].empty())
				m_PlaylistEff[i][m_PlaylistEffIdx[i]].first->FinalTick();
		}
		for (int i = 0; i < (int)EffectLayer::END; ++i)
		{
			if (!m_PlaylistScr[i].empty())
				m_PlaylistScr[i][m_PlaylistScrIdx[i]].first->FinalTick();
		}
	}
	else
	{
		if (GetOwnerObj()->GetRigidBody()->GetAirborne())
		{
			// 공중 대기 상태 - 즉시 점프 상태로 전환
			GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Jump));
		}
		else
		{
			// 지상 대기 상태
			m_IdleTimer += CTimeMgr::GetInst()->GetDeltaTime();
			m_IdleAnimation->FinalTick();
			for (int i = 0; i < 25; ++i)
				if (m_IdleAnimationAvt[i])
					m_IdleAnimationAvt[i]->FinalTick();
			if (m_IdleTimer > 10.f)
			{
				m_IdleTimer = 0.f;
				GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Idle));
			}

			// 대기상태 중 입력에 따라 상태 변경
			if (CKeyMgr::GetInst()->GetKeyState(Keyboard::UP) == Key_state::PRESSED
				|| CKeyMgr::GetInst()->GetKeyState(Keyboard::DOWN) == Key_state::PRESSED
				|| CKeyMgr::GetInst()->GetKeyState(Keyboard::LEFT) == Key_state::PRESSED
				|| CKeyMgr::GetInst()->GetKeyState(Keyboard::RIGHT) == Key_state::PRESSED)
			{
				m_IdleTimer = 0.f;
				GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Walk));
			}
			if (CKeyMgr::GetInst()->GetCommand() == vector<Keyboard>{ Keyboard::LEFT, Keyboard::LEFT }
			|| CKeyMgr::GetInst()->GetCommand() == vector<Keyboard>{ Keyboard::RIGHT, Keyboard::RIGHT })
			{
				m_IdleTimer = 0.f;
				GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Run));
			}
			if (GetOwnerObj()->GetRigidBody()->GetAirborne())
			{
				m_IdleTimer = 0.f;
				GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Jump));
			}

			if (CKeyMgr::GetInst()->GetKeyState(Keyboard::SPACE) == Key_state::TAP)
			{
				GetOwnerObj()->GetRigidBody()->Jump(800.f);
			}
		}
	}

	AttackCheck();
}

void CPlayer_Attack::Exit()
{
	m_CurrentSkill = PlayerSkill::END;
}

void CPlayer_Attack::Render()
{
	// 플레이리스트가 비었으면 기본 대기모션 재생, 있으면 플레이리스트 순차재생
	if (m_Playlist.empty())
	{
		m_IdleAnimation->Render(GetOwnerObj(), false, GetOwnerObj()->IsLookLeft());
		for (int i = 0; i < 25; ++i)
		{
			if (m_IdleAnimationAvt[i])
				m_IdleAnimationAvt[i]->Render(GetOwnerObj(), false, GetOwnerObj()->IsLookLeft());
		}
	}
	else
	{
		// 시간경과 기록
		m_PlaylistTimer += CTimeMgr::GetInst()->GetDeltaTime(); 
		for (int i = 0; i < (int)EffectLayer::END; ++i)
		{
			m_PlaylistEffTimer[i] += CTimeMgr::GetInst()->GetDeltaTime();
		}
		for (int i = 0; i < (int)EffectLayer::END; ++i)
		{
			m_PlaylistScrTimer[i] += CTimeMgr::GetInst()->GetDeltaTime();
		}

		// 스크린 렌더링
		for (int i = 0; i < (int)EffectLayer::END; ++i)
		{
			if (!m_PlaylistScr[i].empty())
				m_PlaylistScr[i][m_PlaylistScrIdx[i]].first->Render(m_Screen, true, GetOwnerObj()->IsLookLeft());
		}

		// 플레이어 스킨 및 아바타 렌더링
		m_Playlist[m_PlaylistIdx].first->Render(GetOwnerObj(), false, GetOwnerObj()->IsLookLeft());
		for (int i = 0; i < 25; ++i)
		{
			if (m_PlaylistAvt.size() > m_PlaylistIdx)
			{
				if (m_PlaylistAvt[m_PlaylistIdx].first[i])
					m_PlaylistAvt[m_PlaylistIdx].first[i]->Render(GetOwnerObj(), false, GetOwnerObj()->IsLookLeft());
			}
		}

		// 이펙트 렌더링
		for (int i = 0; i < (int)EffectLayer::END; ++i)
		{
			if (!m_PlaylistEff[i].empty())
				m_PlaylistEff[i][m_PlaylistEffIdx[i]].first->Render(GetOwnerObj(), false, GetOwnerObj()->IsLookLeft());
		}
		
		// 플레이리스트 다음 씬 계산
		if (m_Playlist[m_PlaylistIdx].second < m_PlaylistTimer)
		{
			m_PlaylistTimer = 0.f;
			if (m_PlaylistIdx < m_Playlist.size() - 1)
			{
				++m_PlaylistIdx;
			}
			else if (m_PlaylistIdx == m_Playlist.size() - 1)
			{
				ClearPlaylist();
			}
		}

		for (int i = 0; i < (int)EffectLayer::END; ++i)
		{
			if (m_PlaylistEff[i].empty())
				continue;
			if (m_PlaylistEff[i][m_PlaylistEffIdx[i]].second < m_PlaylistEffTimer[i])
			{
				m_PlaylistEffTimer[i] = 0.f;
				if (m_PlaylistEffIdx[i] < m_PlaylistEff[i].size() - 1)
				{
					++m_PlaylistEffIdx[i];
				}
				else if (m_PlaylistEffIdx[i] == m_PlaylistEff[i].size() - 1)
				{
					ClearEffPlaylist(i);
				}
			}
		}

		for (int i = 0; i < (int)EffectLayer::END; ++i)
		{
			if (m_PlaylistScr[i].empty())
				continue;
			if (m_PlaylistScr[i][m_PlaylistScrIdx[i]].second < m_PlaylistScrTimer[i])
			{
				m_PlaylistScrTimer[i] = 0.f;
				if (m_PlaylistScrIdx[i] < m_PlaylistScr[i].size() - 1)
				{
					++m_PlaylistScrIdx[i];
				}
				else if (m_PlaylistScrIdx[i] == m_PlaylistScr[i].size() - 1)
				{
					ClearScrPlaylist(i);
				}
			}
		}
	}
}

void CPlayer_Attack::AddPlaylist(CAlbumPlayer* _AP, float _dur)
{
	if (_dur == 0)
	{
		_dur = (float)(_AP->GetFinal() + 1) / _AP->GetFPS() + 0.2f;
	}
	m_Playlist.push_back(make_pair(_AP, _dur));
}

void CPlayer_Attack::AddPlaylistAvt(vector<CAlbumPlayer*> _APVec, float _dur)
{
	if (_dur == 0)
	{
		_dur = (float)(_APVec[0]->GetFinal() + 1) / _APVec[0]->GetFPS() + 0.2f;
	}
	m_PlaylistAvt.push_back(make_pair(_APVec, _dur));
}

void CPlayer_Attack::AddPlaylistEff(CAlbumPlayer* _AP, EffectLayer _layer, float _dur)
{
	if (_dur == 0)
	{
		_dur = (float)(_AP->GetFinal() + 1) / _AP->GetFPS() + 0.2f;
	}
	m_PlaylistEff[(int)_layer].push_back(make_pair(_AP, _dur));
}

void CPlayer_Attack::AddPlaylistScr(CAlbumPlayer* _AP, EffectLayer _layer, float _dur)
{
	if (_dur == 0)
	{
		_dur = (float)(_AP->GetFinal() + 1) / _AP->GetFPS() + 0.2f;
	}
	m_PlaylistScr[(int)_layer].push_back(make_pair(_AP, _dur));
}

void CPlayer_Attack::ClearAllPlaylist()
{
	ClearPlaylist();
	for (int i = 0; i < (int)EffectLayer::END; ++i)
	{
		ClearEffPlaylist(i);
		ClearScrPlaylist(i);
	}
}

void CPlayer_Attack::ClearPlaylist()
{
	for (int i = 0; i < m_Playlist.size(); ++i)
	{
		delete m_Playlist[i].first;
	}
	m_Playlist.clear();

	for (int i = 0; i < m_PlaylistAvt.size(); ++i)
	{
		for (int j = 0; j < 25; ++j)
		{
			delete m_PlaylistAvt[i].first[j];
		}
	}
	m_PlaylistAvt.clear();

	m_PlaylistTimer = 0.f;
	m_PlaylistIdx = 0;
}

void CPlayer_Attack::ClearEffPlaylist(int i)
{
	for (int j = 0; j < m_PlaylistEff[i].size(); ++j)
	{
		delete m_PlaylistEff[i][j].first;
	}
	m_PlaylistEff[i].clear();

	m_PlaylistEffTimer[i] = 0.f;
	m_PlaylistEffIdx[i] = 0;
}

void CPlayer_Attack::ClearScrPlaylist(int i)
{
	for (int j = 0; j < m_PlaylistScr[i].size(); ++j)
	{
		delete m_PlaylistScr[i][j].first;
	}
	m_PlaylistScr[i].clear();

	m_PlaylistScrTimer[i] = 0.f;
	m_PlaylistScrIdx[i] = 0;
}

bool CPlayer_Attack::AttackCheck()
{
	for (int i = 0; i < AttackKey.size(); ++i)
	{
		if (CKeyMgr::GetInst()->GetKeyState(AttackKey[i]) == Key_state::TAP)
		{
			m_TappedKey = AttackKey[i];
			return true;
		}
	}
	m_TappedKey = Keyboard::Keyboard_end;
	return false;
}

void CPlayer_Attack::GeardriveCallback(DWORD_PTR _arrow)
{
	CPlayerSkill* pSkill = new CPlayerSkill(L"GeardriveExplosion", 0.5f);
	pSkill->SetScale(Vec2D(260, 160));
	pSkill->SetLocation(((CArrow*)_arrow)->GetLocation() - (pSkill->GetScale() / 2) + Vec2D(0, -40));
	CCollider* pCol = new CCollider(L"GeardriveExplosion_Col");
	pSkill->RegisterBodyCollider(pCol);
	pSkill->AddComponent(pCol);
	pSkill->SetATK(500);
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pSkill, LayerType::Object);
}

void CPlayer_Attack::MHRArrowCyan(int _num)
{
	int _angle = -30 - (3 * _num);
	int spd = 1000;
	CArrow* pArrow = new CArrow(std::to_wstring(_num)
		, Vec2D(GetOwnerObj()->IsLookLeft() ? -(spd * cosf(_angle * PI / 180)) : (spd * cosf(_angle * PI / 180)), 0.f)
		, -50 + GetOwnerObj()->GetRigidBody()->GetAirborneHeight(), 1.f);
	pArrow->GetRigidBody()->SetAirborneSpeed(spd * sinf(_angle * PI / 180));
	pArrow->SetLocation(GetOwnerObj()->GetLocation() + GetOwnerObj()->GetScale() / 2.f);
	pArrow->GetRigidBody()->SetZeroGravity(true);
	pArrow->SetIdleAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_mistheavyrain_arrowcyan"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_mistheavyrain_arrowcyan.animation"));
	pArrow->SetDeadFunc(this, (DELEGATE_1)&CPlayer_Attack::MHRArrowCyanCallback0);
	pArrow->SetPierceCount(100);
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pArrow, LayerType::Object);
}

void CPlayer_Attack::MHRArrowCyanCallback0(DWORD_PTR _arrow)
{
	CArrow* pPrevArrow = (CArrow*)_arrow;
	int spd = -pPrevArrow->GetRigidBody()->GetAirborneHeight() * 2;
	int offsetX = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetGroundPos().x - pPrevArrow->GetGroundPos().x;
	offsetX = (offsetX / abs(offsetX)) * 200;
	CArrow* pArrow = new CArrow(L"MHR_Arrow", Vec2D(0, 0)
		, -50 + GetOwnerObj()->GetRigidBody()->GetAirborneHeight(), 3);
	pArrow->GetRigidBody()->SetAirborneSpeed(spd);
	pArrow->GetRigidBody()->SetAirborneHeight(pPrevArrow->GetRigidBody()->GetAirborneHeight());
	pArrow->SetGroundPos(Vec2D(pPrevArrow->GetGroundPos().x + offsetX
		, pPrevArrow->GetGroundPos().y + 100 * ((std::stoi(pPrevArrow->GetName()) + 2) % 3 - 1)));
	pArrow->GetRigidBody()->SetZeroGravity(true);
	pArrow->SetIdleAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_mistheavyrain_arrowcyan"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_mistheavyrain_arrowcyan.animation"));
	pArrow->SetDeadAniG(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_mistheavyrain_expcyan"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_mistheavyrain_expcyan.animation", Vec2D(0, -15)), ArrowEffLayer::a);
	pArrow->SetDeadSound(CSoundMgr::GetInst()->GetSound(L"mist_heavy_rain_impact_03", L"\\sound\\sounds_char_archer_effect\\mist_heavy_rain_impact_03.ogg"));
	pArrow->SetDeadFunc(this, (DELEGATE_1)&CPlayer_Attack::MHRArrowCyanCallback1);
	pArrow->SetPierceCount(100);
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pArrow, LayerType::Object);
}

void CPlayer_Attack::MHRArrowCyanCallback1(DWORD_PTR _arrow)
{
	CPlayerAoE* pAoE = new CPlayerAoE(L"MHRExplosion", 120.f);
	pAoE->SetLocation(((CArrow*)_arrow)->GetLocation());
	pAoE->SetEffAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_mistheavyrain_crack"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_mistheavyrain_crack.animation"), AoEEffLayer::a);
	pAoE->SetEffAni(CAlbumPlayer::CreatePlayerFromFile(L"archer_effect_mistheavyrain_crackcyan"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_effect_mistheavyrain_crackcyan.animation"), AoEEffLayer::b);
	pAoE->SetATK(500);
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pAoE, LayerType::Object);
}
