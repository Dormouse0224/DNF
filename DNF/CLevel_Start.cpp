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

	string album1 = "sprite/interface2/channel/serverselect/background/background.img";
	wstring directory1 = L"D:\\repos\\DNF\\Output\\resource\\texture\\ui\\sprite_interface2_channel_serverselect_background.NPK";
	CAlbumPlayer* albumplayer1 = new CAlbumPlayer(L"StartBackground", album1, directory1);
	pStartBackground->AddComponent(albumplayer1);
	albumplayer1->SetPlayInfo(0, 0, true, 0, Vec2D(0, 0));

	string album2 = "sprite/character/archer/equipment/avatar/skin/ac_body0010.img";
	wstring directory2 = L"D:\\repos\\DNF\\Output\\resource\\texture\\player\\sprite_character_archer_equipment_avatar_skin.NPK";
	CAlbumPlayer* albumplayer2 = new CAlbumPlayer(L"Cutin", album2, directory2);
	pStartBackground->AddComponent(albumplayer2);
	albumplayer2->SetPlayInfo(12, 19, true, 10, Vec2D(0, 0));

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
