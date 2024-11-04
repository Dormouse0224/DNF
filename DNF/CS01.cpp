#include "pch.h"
#include "CS01.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CPlayer.h"
#include "CTimeMgr.h"
#include "CRigidBody.h"
#include "CCameraMgr.h"
#include "CDummy.h"
#include "CLevelMgr.h"
#include "CSoundMgr.h"
#include "CSound.h"

CS01::CS01()
	: CStage(L"S01")
	, m_Duration(0)
	, m_PlayerIncome(false)
	, m_toDeleteId(0)
	, m_scene01(false)
{
	m_StageInfo = new StageInfo();
	m_StageInfo->StageSize = Vec2D(1110, 1000);
	m_StageInfo->StageName = GetName();
}

CS01::~CS01()
{
}

void CS01::Begin()
{
	SetCameraFollowPlayer(true);
	CCameraMgr::GetInst()->SetEffect(CameraEffect::FadeIn);

	// 배경 객체 생성 후 애니메이션 입력
	CBackground* pBackground = new CBackground(GetName() + L"_Background_0");
	pBackground->SetScale(m_StageInfo->StageSize);
	AddObject(pBackground, LayerType::Background);
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_deceitfulwatchtower_sky_background.animation", Vec2D(0, 0)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_deceitfulwatchtower_sea_background.animation", Vec2D(0, 200)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_deceitfulwatchtower_watchtower_eye.animation", Vec2D(170, 0)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_deceitfulwatchtower_watchtower.animation", Vec2D(570, 50)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_deceitfulwatchtower_floatingisland.animation", Vec2D(0, 0)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_deceitfulwatchtower_waterfall.animation", Vec2D(391, 254)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_deceitfulwatchtower_cliff_1.animation", Vec2D(300, 600)));
	for (int i = 0; i < 4; ++i)
	{
		pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
			+ L"\\animation\\map_deceitfulwatchtower_tile_" + std::to_wstring(i) + L".animation", Vec2D(290 + (i * 220), 3)));
	}

	CDummy* pDummy = new CDummy(L"boss_treacherous0");
	pDummy->SetLocation(Vec2D(700, 350));
	pDummy->SetScale(Vec2D(260, 210));
	pDummy->SetLookLeft(true);
	AddObject(pDummy, LayerType::Object);
	pDummy->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pDummy->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\boss_treacherous_down_0.animation"));
	m_toDeleteId = pDummy->GetID();

	CBackground* pWait3 = new CBackground(L"s01_frontstone");
	pWait3->SetLocation(Vec2D(100, 50));
	m_WaitList.push_back(pWait3);
	pWait3->SetScale(Vec2D(874, 500));
	pWait3->SetFallowCam(true);
	pWait3->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pWait3->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\s01_frontstone.animation"));



	// 플레이어 추가
	CPlayer* pPlayer = new CPlayer();
	AddObject(pPlayer, LayerType::Object);
	SetPlayer(pPlayer);
	pPlayer->SetLocation(Vec2D(100, 1100));

	SetGravity(0);
	pPlayer->GetRigidBody()->SetAirborne(true);
}

void CS01::Tick()
{
	if (!m_PlayerIncome)
	{
		if (m_Duration < 3)
		{
			m_Duration += CTimeMgr::GetInst()->GetDeltaTime();
			GetPlayer()->SetLocation(Vec2D(150 * m_Duration + 100, 300 * powf(m_Duration - 2, 2) + 100));
		}
		else
		{
			SetGravity(1500);
			GetPlayer()->GetRigidBody()->SetAirborne(false);
			m_PlayerIncome = true;
			m_Duration = 0;
			CSoundMgr::GetInst()->GetSound(L"largo_3rd_vo_die_02"
				, L"\\sound\\sounds_mon_duskyisland_vo\\largo_3rd_vo_die_02.ogg")->Play();
		}
	}
	else
	{
		if (!m_scene01)
		{
			m_Duration += CTimeMgr::GetInst()->GetDeltaTime();
			if (m_Duration > 7)
			{
				
				DeleteObject(LayerType::Object, m_toDeleteId);
				AddObject(m_WaitList[0], LayerType::Near);
				CCameraMgr::GetInst()->SetEffect(CameraEffect::FadeOut);
				m_scene01 = true;
			}
		}
		else
		{
			if (CCameraMgr::GetInst()->IsEffectFin())
			{
				CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_meet_01"
					, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_meet_01.ogg")->Play();
				CSound* pSound = CSoundMgr::GetInst()->GetSound(L"duskyisland_largo_3rdform_3phase"
					, L"\\music\\duskyisland_largo_3rdform_3phase.ogg");
				pSound->SetVolume(10);
				pSound->PlayToBGM(true);
				CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"S03"));
			}
		}
	}




	CLevel::Tick();
}

void CS01::FinalTick()
{

	CLevel::FinalTick();
}

void CS01::Render()
{

	CLevel::Render();
}

void CS01::End()
{

	ClearAll();
}
