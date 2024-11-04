#include "pch.h"
#include "CS03.h"
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
#include "CDummy.h"

CS03::CS03()
	: CStage(L"S03")
{
	m_StageInfo = new StageInfo();
	m_StageInfo->StageSize = Vec2D(1568, 960);
	m_StageInfo->StageName = GetName();
}

CS03::~CS03()
{
}

void CS03::Begin()
{
	CCameraMgr::GetInst()->InitCameraPos();
	CCameraMgr::GetInst()->SetEffect(CameraEffect::FadeIn);

	// 배경 객체 생성 후 애니메이션 입력
	CBackground* pBackground = new CBackground(GetName() + L"_Background_0");
	pBackground->SetScale(m_StageInfo->StageSize);
	AddObject(pBackground, LayerType::Background);
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_brokenwatchtower_background.animation", Vec2D(0, 0)));

	CDummy* pDummy0 = new CDummy(L"Dummy0");
	pDummy0->SetScale(Vec2D(431, 200));
	pDummy0->SetLocation(Vec2D(0, 360));
	AddObject(pDummy0, LayerType::Object);
	pDummy0->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pDummy0->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_brokenwatchtower_obj_0.animation", Vec2D(0, 0)));

	CDummy* pDummy1 = new CDummy(L"Dummy1");
	pDummy1->SetScale(Vec2D(590, 300));
	pDummy1->SetLocation(Vec2D(1050, 620));
	AddObject(pDummy1, LayerType::Object);
	pDummy1->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pDummy1->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_brokenwatchtower_obj_1.animation", Vec2D(0, 0)));





	// 플레이어 추가
	CPlayer* pPlayer = new CPlayer();
	AddObject(pPlayer, LayerType::Object);
	SetPlayer(pPlayer);
	pPlayer->SetLocation(Vec2D(1000, 500));


	SetCameraFollowPlayer(true);
}

void CS03::Tick()
{

	CLevel::Tick();
}

void CS03::FinalTick()
{

	CLevel::FinalTick();
}

void CS03::Render()
{

	CLevel::Render();
}

void CS03::End()
{

	ClearAll();
}

