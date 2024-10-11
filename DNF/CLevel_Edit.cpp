#include "pch.h"
#include "CEngine.h"
#include "resource.h"
#include "CLevel_Edit.h"
#include "CLevelMgr.h"
#include "CTextMgr.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"

CLevel_Edit::CLevel_Edit()
	:CLevel(L"Level_Edit")
{
}

CLevel_Edit::~CLevel_Edit()
{
}

void CLevel_Edit::Begin()
{
	CBackground* pStartBackground = new CBackground(L"StartBackground");

	string album = "sprite/character/archer/equipment/avatar/skin/ac_body0000.img";
	wstring directory = L"D:\\repos\\DNF\\Output\\resource\\texture\\player\\sprite_character_archer_equipment_avatar_skin.NPK";
	CAlbumPlayer* albumplayer = new CAlbumPlayer(L"Cutin1", album, directory);
	pStartBackground->AddComponent(albumplayer);
	albumplayer->SetPlayInfo(12, 19, true, 10, Vec2D(0, 0));

	album = "sprite/character/archer/equipment/avatar/skin/ac_body0001.img";
	directory = L"D:\\repos\\DNF\\Output\\resource\\texture\\player\\sprite_character_archer_equipment_avatar_skin.NPK";
	CAlbumPlayer* albumplayer1 = new CAlbumPlayer(L"Cutin2", album, directory);
	pStartBackground->AddComponent(albumplayer1);
	albumplayer1->SetPlayInfo(12, 19, true, 10, Vec2D(100, 0));

	album = "sprite/character/archer/equipment/avatar/skin/ac_body0002.img";
	directory = L"D:\\repos\\DNF\\Output\\resource\\texture\\player\\sprite_character_archer_equipment_avatar_skin.NPK";
	CAlbumPlayer* albumplayer2 = new CAlbumPlayer(L"Cutin3", album, directory);
	pStartBackground->AddComponent(albumplayer2);
	albumplayer2->SetPlayInfo(12, 19, true, 10, Vec2D(200, 0));

	AddObject(pStartBackground, LayerType::Background);

	CTextMgr::GetInst()->WriteText(10, 10, L"에디터 레벨", RGB(0, 0, 0));

	CLevel::Begin();
}

void CLevel_Edit::Tick()
{
	CLevel::Tick();
}

void CLevel_Edit::FinalTick()
{
	CLevel::FinalTick();
}

void CLevel_Edit::Render()
{
	CLevel::Render();
}

void CLevel_Edit::End()
{
}


bool EditorMenu(HINSTANCE _hInst, HWND _hWnd, int _wmID)
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (!dynamic_cast<CLevel_Edit*>(pLevel))
		return false;

	switch (_wmID)
	{
	case ID_RES600:
	{

		CEngine::GetInst()->SetResolution(Vec2D(1066, 600));
		CEngine::GetInst()->SetScreenScale(1);
		CEngine::GetInst()->ChangeWindowSize(Vec2D(CEngine::GetInst()->GetResolution().x, CEngine::GetInst()->GetResolution().y));
		return true;
	}
	case ID_RES900:
	{
		CEngine::GetInst()->SetResolution(Vec2D(1600, 900));
		CEngine::GetInst()->SetScreenScale(1.5);
		CEngine::GetInst()->ChangeWindowSize(Vec2D(CEngine::GetInst()->GetResolution().x, CEngine::GetInst()->GetResolution().y));
		return true;
	}
	case ID_RES1200:
	{
		CEngine::GetInst()->SetResolution(Vec2D(2133, 1200));
		CEngine::GetInst()->SetScreenScale(2);
		CEngine::GetInst()->ChangeWindowSize(Vec2D(CEngine::GetInst()->GetResolution().x, CEngine::GetInst()->GetResolution().y));
		return true;
	}
	}
	return false;
}