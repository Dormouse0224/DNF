#include "pch.h"
#include "CLevel_Start.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"

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
	string album = "sprite/interface2/channel/serverselect/background/background.img";
	wstring directory = L"D:\\repos\\DNF\\Output\\resource\\texture\\ui\\sprite_interface2_channel_serverselect_background.NPK";
	CAlbumPlayer* albumplayer = new CAlbumPlayer(L"StartBackground", album, directory);
	pStartBackground->AddComponent(albumplayer);
	albumplayer->SetPlayInfo(0, 0, true, 0);
	AddObject(pStartBackground, LayerType::Background);


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
