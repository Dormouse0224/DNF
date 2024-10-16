#include "pch.h"
#include "CLevel_Start.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CTextMgr.h"

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
	pStartBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"temp", "D:\\repos\\DNF\\Output\\resource\\animation\\running_archor_test.animation"));

	AddObject(pStartBackground, LayerType::Background);

	CTextMgr::GetInst()->WriteText(10, 10, L"스타트 레벨", RGB(255, 255, 255));

	CLevel::Begin();
}

void CLevel_Start::Tick()
{
	CLevel::Tick();
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
}
