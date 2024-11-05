#include "pch.h"
#include "CPurpleSeaRoad.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CPlayer.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CCameraMgr.h"
#include "CRigidBody.h"

#include "CTextureMgr.h"

CPurpleSeaRoad::CPurpleSeaRoad()
	: CStage(L"PurpleSeaRoad")
{
	m_StageInfo = new StageInfo();
	m_StageInfo->StageSize = Vec2D(1700, 630);
	m_StageInfo->StageName = GetName();
	m_StageInfo->BGMPath = L"duskyisland_purplesearoad.ogg";
}

CPurpleSeaRoad::~CPurpleSeaRoad()
{
}

void CPurpleSeaRoad::Begin()
{
	CTextureMgr::PreloadFromFile(L"BossResources.txt");

	SetCameraFollowPlayer(true);

	// 배경 객체 생성 후 애니메이션 입력
	CBackground* pBackground = new CBackground(GetName() + L"_Background_0");
	pBackground->SetScale(m_StageInfo->StageSize);
	AddObject(pBackground, LayerType::Background);
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_purplesearoad_background.animation", Vec2D(955, 0)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_purplesearoad_background.animation"));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_purplesearoad_rockymountain.animation", Vec2D(1200, 0)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_purplesearoad_topwave_1.animation", Vec2D(536, 10)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_purplesearoad_topwave_0.animation"));
	for (int i = -120; i < m_StageInfo->StageSize.x; i += 672)
	{
		pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
			+ L"\\animation\\map_purplesearoad_middlewave.animation", Vec2D(i, (int)m_StageInfo->StageSize.y - 367)));
	}
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_purplesearoad_mistspinback.animation", Vec2D(1200, 300)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_purplesearoad_speedmist.animation", Vec2D(1200, 200)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_purplesearoad_speedmist.animation", Vec2D(1200, 0)));
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_purplesearoad_mistspinfront.animation", Vec2D(1200, 320)));


	// 플레이어 추가
	CPlayer* pPlayer = new CPlayer();
	AddObject(pPlayer, LayerType::Object);
	SetPlayer(pPlayer);
	pPlayer->SetLocation(Vec2D(100, 450));

	// BGM이 있으면 재생
	if (!m_StageInfo->BGMPath.empty())
	{
		m_BGM = CSoundMgr::GetInst()->GetSound(m_StageInfo->BGMPath, L"\\music\\" + m_StageInfo->BGMPath);
		m_BGM->PlayToBGM(true);
	}
}

void CPurpleSeaRoad::Tick()
{
	if (GetPlayer()->GetGroundPos() >> Vec2D(1200, 320) && GetPlayer()->GetGroundPos() << Vec2D(1550, 470))
		m_Timer += CTimeMgr::GetInst()->GetDeltaTime();

	if (m_Timer > 3.f)
	{
		m_Timer = 0;
		CCameraMgr::GetInst()->SetEffect(CameraEffect::FadeOut);
		SetGravity(0);
		GetPlayer()->GetRigidBody()->Jump(700.f);
	}

	if (CCameraMgr::GetInst()->IsEffectFin())
	{
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"Cliff"));
	}

	CLevel::Tick();
}

void CPurpleSeaRoad::FinalTick()
{

	CLevel::FinalTick();
}

void CPurpleSeaRoad::Render()
{

	CLevel::Render();
}

void CPurpleSeaRoad::End()
{
	ClearAll();
}
