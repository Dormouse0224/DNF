#include "pch.h"
#include "CLevel_Start.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CTextMgr.h"
#include "CKeyMgr.h"
#include "CLevelMgr.h"
#include "CSoundMgr.h"
#include "CSound.h"

CLevel_Start::CLevel_Start()
	:CLevel(L"Level_Start")
{
}

CLevel_Start::~CLevel_Start()
{
}

void CLevel_Start::Begin()
{
	// 시작 레벨의 배경 생성
	CBackground* pStartBackground = new CBackground(L"StartBackground");

	pStartBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"temp", "D:\\repos\\DNF\\Output\\resource\\animation\\start_background.animation"));
	pStartBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"temp", "D:\\repos\\DNF\\Output\\resource\\animation\\archer_walk_30.animation"));
	pStartBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"temp", "D:\\repos\\DNF\\Output\\resource\\animation\\archer_walk_-30.animation"));
	pStartBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"temp", "D:\\repos\\DNF\\Output\\resource\\animation\\archer_walk_0.animation"));
	pStartBackground->SetScale(Vec2D(1066, 600));
	AddObject(pStartBackground, LayerType::Background);

	CTextMgr::GetInst()->WriteText(10, 10, L"스타트 레벨", RGB(255, 255, 255));

	// 시작 레벨 BGM 세팅
	CSound* pStartBGM = CSoundMgr::GetInst()->GetSound(L"StartBGM", L"\\music\\character_stage.ogg");
	pStartBGM->PlayToBGM(true);

	CLevel::Begin();
}

void CLevel_Start::Tick()
{
	CLevel::Tick();
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::LCTRL) == Key_state::PRESSED && CKeyMgr::GetInst()->GetKeyState(Keyboard::E) == Key_state::PRESSED)
	{
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"Level_Edit"));
	}
}

void CLevel_Start::FinalTick()
{
	CLevel::FinalTick();
}

void CLevel_Start::Render()
{
	CLevel::Render();
}

void CLevel_Start::End()
{
	ClearObject();
	CTextMgr::GetInst()->DeleteText(10, 10, L"스타트 레벨");
}
