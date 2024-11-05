#include "pch.h"
#include "CCliff.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CPlayer.h"
#include "CTimeMgr.h"
#include "CRigidBody.h"
#include "CCameraMgr.h"
#include "CLevelMgr.h"
#include "CSoundMgr.h"
#include "CSound.h"

CCliff::CCliff()
	: CStage(L"Cliff")
	, m_Cliff{nullptr}
	, m_Duration(0.f)
	, m_fadeout(false)
{
	m_StageInfo = new StageInfo();
	m_StageInfo->StageSize = Vec2D(1066, 600);
	m_StageInfo->StageName = GetName();
}

CCliff::~CCliff()
{
}

void CCliff::Begin()
{
	CCameraMgr::GetInst()->InitCameraPos();
	CCameraMgr::GetInst()->SetEffect(CameraEffect::FadeIn);

	// 배경 객체 생성 후 애니메이션 입력
	CBackground* pBackground = new CBackground(GetName() + L"_Background_0");
	pBackground->SetScale(m_StageInfo->StageSize);
	AddObject(pBackground, LayerType::Background);
	for (int i = 0; i < 1066; i += 223)
	{
		pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
			+ L"\\animation\\map_deceitfulwatchtower_cliff_background.animation", Vec2D(i, 0)));
	}
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_deceitfulwatchtower_cliff_background_d.animation", Vec2D(0, 0)));
	m_Cliff[0] = CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_deceitfulwatchtower_cliff_0.animation", Vec2D(750, 0));
	pBackground->AddComponent(m_Cliff[0]);
	m_Cliff[1] = CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_deceitfulwatchtower_cliff_0.animation", Vec2D(750, 0));
	pBackground->AddComponent(m_Cliff[1]);
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_deceitfulwatchtower_fall.animation", Vec2D(0, 0)));



	// 플레이어 추가
	CPlayer* pPlayer = new CPlayer();
	AddObject(pPlayer, LayerType::Object);
	SetPlayer(pPlayer);

	SetGravity(0);
	pPlayer->GetRigidBody()->SetAirborne(true);
	pPlayer->SetLocation(Vec2D(750, 700));

	CSound* pSound = CSoundMgr::GetInst()->GetSound(L"brim_mgaic_wind_loop_st"
		, L"\\sound\\sounds_mon_duskyisland\\brim_mgaic_wind_loop_st.ogg");
	pSound->SetVolume(10);
	pSound->PlayToBGM(true);
}

void CCliff::Tick()
{
	for (int i = 0; i < 2; ++i)
	{
		m_Cliff[i]->SetOffset(Vec2D(750, ((int)(m_Cliff[i]->GetOffset().y + (720 * i) + m_Speed * CTimeMgr::GetInst()->GetDeltaTime()) % 720) - (720 * i)));
	}
	CLevel::Tick();

	m_Duration += CTimeMgr::GetInst()->GetDeltaTime();
	GetPlayer()->SetLocation(Vec2D(750.f, (300.f - 50.f * powf(m_Duration - 4.f, 3.f))));

	if (GetPlayer()->GetLocation().y < -100)
	{
		CCameraMgr::GetInst()->SetEffect(CameraEffect::FadeOut);
		m_fadeout = true;
	}
	if (CCameraMgr::GetInst()->IsEffectFin() && m_fadeout)
	{
		CSoundMgr::GetInst()->GetSound(L"brim_mgaic_wind_loop_st"
			, L"\\sound\\sounds_mon_duskyisland_vo\\brim_mgaic_wind_loop_st.ogg")->Stop(true);
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"S01"));
	}
}

void CCliff::FinalTick()
{
	CLevel::FinalTick();
}

void CCliff::Render()
{
	CLevel::Render();
}

void CCliff::End()
{
	ClearAll();
}
