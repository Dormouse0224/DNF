#include "pch.h"
#include "CEngine.h"
#include "resource.h"
#include "CLevel_Edit.h"
#include "CLevelMgr.h"
#include "CTextMgr.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CTextureMgr.h"
#include "CButton_LoadNPK.h"

CLevel_Edit::CLevel_Edit()
	:CLevel(L"Level_Edit")
{
}

CLevel_Edit::~CLevel_Edit()
{
}

void CLevel_Edit::Begin()
{
	// 배경 오브젝트 추가
	CBackground* pStartBackground = new CBackground(L"StartBackground");
	AddObject(pStartBackground, LayerType::Background);

	//string album2 = "sprite/character/archer/equipment/avatar/skin/ac_body0010.img";
	//wstring directory2 = L"D:\\repos\\DNF\\Output\\resource\\texture\\player\\sprite_character_archer_equipment_avatar_skin.NPK";
	//CAlbumPlayer* albumplayer2 = new CAlbumPlayer(L"Cutin", album2, directory2);
	//pStartBackground->AddComponent(albumplayer2);
	//albumplayer2->SetPlayInfo(12, 19, true, 10, Vec2D(0, 0));

	//string album = "ZZZ";
	//wstring directory = L"D:\\Img\\ZZZ\\ZZZ.NPK";
	//CAlbumPlayer* albumplayer = new CAlbumPlayer(L"ZZZ", album, directory);
	//pStartBackground->AddComponent(albumplayer);
	//albumplayer->SetPlayInfo(0, 0, true, 0, Vec2D(50, 100));

	//CTextureMgr::GetInst()->LoadFromFile(L"C:\\Users\\arcti\\Desktop\\temp_res\\0.png");
	//CTextureMgr::GetInst()->LoadFromFile(L"C:\\Users\\arcti\\Desktop\\temp_res\\1.png");
	//CTextureMgr::GetInst()->LoadFromFile(L"C:\\Users\\arcti\\Desktop\\temp_res\\2.png");
	//CTextureMgr::GetInst()->LoadFromFile(L"C:\\Users\\arcti\\Desktop\\temp_res\\3.png");
	//CTextureMgr::GetInst()->LoadFromFile(L"C:\\Users\\arcti\\Desktop\\temp_res\\4.png");
	//CTextureMgr::GetInst()->LoadFromFile(L"C:\\Users\\arcti\\Desktop\\temp_res\\5.png");
	//CTextureMgr::GetInst()->LoadFromFile(L"C:\\Users\\arcti\\Desktop\\temp_res\\6.png");
	//CTextureMgr::GetInst()->LoadFromFile(L"C:\\Users\\arcti\\Desktop\\temp_res\\7.png");
	//CTextureMgr::GetInst()->LoadFromFile(L"C:\\Users\\arcti\\Desktop\\temp_res\\8.png");
	//CAlbumPlayer* albumplayer3 = new CAlbumPlayer(L"pngs", _TempAlbum, _TempNPK);
	//pStartBackground->AddComponent(albumplayer3);
	//albumplayer3->SetPlayInfo(0, 8, true, 1, Vec2D(10, 10));

	//CTextureMgr::GetInst()->SaveAlbum("Edit_Button", "C:\\Users\\arcti\\Desktop\\temp_res");

	CButton_LoadNPK* OpenNpk = new CButton_LoadNPK(L"Btn_OpenNpk");
	CAlbumPlayer* Btn_OpenNpk_Idle = new CAlbumPlayer(L"Btn_OpenNpk_Idle", "Edit_Button"
		, L"D:\\repos\\DNF\\Output\\resource\\texture\\ui\\Edit_Button.NPK");
	OpenNpk->SetAction(Btn_OpenNpk_Idle, BtnState::IDLE);
	Btn_OpenNpk_Idle->SetPlayInfo(0, 0, false, 0, Vec2D(0, 0));
	CAlbumPlayer* Btn_OpenNpk_CursorOn = new CAlbumPlayer(L"Btn_OpenNpk_Idle", "Edit_Button"
		, L"D:\\repos\\DNF\\Output\\resource\\texture\\ui\\Edit_Button.NPK");
	OpenNpk->SetAction(Btn_OpenNpk_CursorOn, BtnState::CURSOR_ON);
	Btn_OpenNpk_CursorOn->SetPlayInfo(1, 1, false, 0, Vec2D(0, 0));
	CAlbumPlayer* Btn_OpenNpk_Pressed = new CAlbumPlayer(L"Btn_OpenNpk_Idle", "Edit_Button"
		, L"D:\\repos\\DNF\\Output\\resource\\texture\\ui\\Edit_Button.NPK");
	OpenNpk->SetAction(Btn_OpenNpk_Pressed, BtnState::PRESSED);
	Btn_OpenNpk_Pressed->SetPlayInfo(2, 2, false, 0, Vec2D(0, 0));

	AddObject(OpenNpk, LayerType::UI);
	OpenNpk->SetScale(Vec2D(219, 47));
	OpenNpk->SetLocation(Vec2D(830, 10));

	// 에디터 레벨 텍스트 추가
	CTextMgr::GetInst()->WriteText(10, 10, L"에디터 레벨", RGB(0, 0, 0));

	// 불러오기 버튼 추가

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
		CTextMgr::GetInst()->ChangeTextSize(CEngine::GetInst()->GetResolution().y / 20);
		return true;
	}
	case ID_RES900:
	{
		CEngine::GetInst()->SetResolution(Vec2D(1600, 900));
		CEngine::GetInst()->SetScreenScale(1.5);
		CEngine::GetInst()->ChangeWindowSize(Vec2D(CEngine::GetInst()->GetResolution().x, CEngine::GetInst()->GetResolution().y));
		CTextMgr::GetInst()->ChangeTextSize(CEngine::GetInst()->GetResolution().y / 20);
		return true;
	}
	case ID_RES1200:
	{
		CEngine::GetInst()->SetResolution(Vec2D(2133, 1200));
		CEngine::GetInst()->SetScreenScale(2);
		CEngine::GetInst()->ChangeWindowSize(Vec2D(CEngine::GetInst()->GetResolution().x, CEngine::GetInst()->GetResolution().y));
		CTextMgr::GetInst()->ChangeTextSize(CEngine::GetInst()->GetResolution().y / 20);
		return true;
	}
	}
	return false;
}