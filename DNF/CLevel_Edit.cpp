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
#include "CTexture.h"
#include "CButton_CreateAlbum.h"

CLevel_Edit::CLevel_Edit()
	:CLevel(L"Level_Edit")
	, m_PreviewTexture(nullptr)
{
}

CLevel_Edit::~CLevel_Edit()
{

}

void CLevel_Edit::Begin()
{
	// NPK 파일 로드 버튼 추가
	CButton_LoadNPK* OpenNpk = new CButton_LoadNPK(L"Btn_OpenNpk");
	CAlbumPlayer* Btn_OpenNpk_Idle = new CAlbumPlayer(L"Btn_OpenNpk_Idle", "Edit_Button"
		, CEngine::GetInst()->GetResourcePath() + L"\\texture\\ui\\Edit_Button.NPK");
	OpenNpk->AddComponent_Btn(Btn_OpenNpk_Idle, BtnState::IDLE);
	Btn_OpenNpk_Idle->SetPlayInfo(0, 0, false, 0, Vec2D(0, 0));
	CAlbumPlayer* Btn_OpenNpk_CursorOn = new CAlbumPlayer(L"Btn_OpenNpk_CursorOn", "Edit_Button"
		, CEngine::GetInst()->GetResourcePath() + L"\\texture\\ui\\Edit_Button.NPK");
	OpenNpk->AddComponent_Btn(Btn_OpenNpk_CursorOn, BtnState::CURSOR_ON);
	Btn_OpenNpk_CursorOn->SetPlayInfo(1, 1, false, 0, Vec2D(0, 0));
	CAlbumPlayer* Btn_OpenNpk_Pressed = new CAlbumPlayer(L"Btn_OpenNpk_Pressed", "Edit_Button"
		, CEngine::GetInst()->GetResourcePath() + L"\\texture\\ui\\Edit_Button.NPK");
	OpenNpk->AddComponent_Btn(Btn_OpenNpk_Pressed, BtnState::PRESSED);
	Btn_OpenNpk_Pressed->SetPlayInfo(2, 2, false, 0, Vec2D(0, 0));

	AddObject(OpenNpk, LayerType::UI);
	OpenNpk->SetScale(Vec2D(219, 47));
	OpenNpk->SetLocation(Vec2D(830, 10));

	// 이미지 파일 로드 버튼 추가
	CButton_CreateAlbum* MakeAlbum = new CButton_CreateAlbum(L"Btn_MakeAlbum");
	CAlbumPlayer* Btn_MakeAlbum_Idle = new CAlbumPlayer(L"Btn_MakeAlbum_Idle", "Edit_Button"
		, CEngine::GetInst()->GetResourcePath() + L"\\texture\\ui\\Edit_Button.NPK");
	MakeAlbum->AddComponent_Btn(Btn_MakeAlbum_Idle, BtnState::IDLE);
	Btn_MakeAlbum_Idle->SetPlayInfo(3, 3, false, 0, Vec2D(0, 0));
	CAlbumPlayer* Btn_MakeAlbum_CursorOn = new CAlbumPlayer(L"Btn_MakeAlbum_CursorOn", "Edit_Button"
		, CEngine::GetInst()->GetResourcePath() + L"\\texture\\ui\\Edit_Button.NPK");
	MakeAlbum->AddComponent_Btn(Btn_MakeAlbum_CursorOn, BtnState::CURSOR_ON);
	Btn_MakeAlbum_CursorOn->SetPlayInfo(4, 4, false, 0, Vec2D(0, 0));
	CAlbumPlayer* Btn_MakeAlbum_Pressed = new CAlbumPlayer(L"Btn_MakeAlbum_Pressed", "Edit_Button"
		, CEngine::GetInst()->GetResourcePath() + L"\\texture\\ui\\Edit_Button.NPK");
	MakeAlbum->AddComponent_Btn(Btn_MakeAlbum_Pressed, BtnState::PRESSED);
	Btn_MakeAlbum_Pressed->SetPlayInfo(5, 5, false, 0, Vec2D(0, 0));

	AddObject(MakeAlbum, LayerType::UI);
	MakeAlbum->SetScale(Vec2D(219, 47));
	MakeAlbum->SetLocation(Vec2D(830, 60));


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
	if (m_PreviewTexture != nullptr)
		m_PreviewTexture->DirectRender();
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