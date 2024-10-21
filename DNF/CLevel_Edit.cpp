#include "pch.h"
#include "CEngine.h"
#include "resource.h"
#include "CLevel_Edit.h"
#include "CLevelMgr.h"
#include "CTextMgr.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CTextureMgr.h"
#include "DialogProc.h"
#include "CTexture.h"
#include "CButton.h"
#include "resource.h"
#include "CKeyMgr.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CCameraMgr.h"

INT_PTR CALLBACK AlbumViewerProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);
INT_PTR CALLBACK CreateAniProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);

CLevel_Edit::CLevel_Edit()
	:CLevel(L"Level_Edit")
	, m_PreviewTexture(nullptr)
	, m_PreviewPlayer(nullptr)
	, m_hAlbumViewerWnd(nullptr)
	, m_hCreateAlbum(nullptr)
	, m_hEditAnimation(nullptr)
{
}

CLevel_Edit::~CLevel_Edit()
{
	if (m_PreviewPlayer)
	{
		delete m_PreviewPlayer;
		m_PreviewPlayer = nullptr;
	}
}

void CLevel_Edit::Begin()
{
	// 카메라 위치 초기화
	CCameraMgr::GetInst()->InitCameraPos();

	// NPK 파일 로드 버튼 추가
	CButton* OpenNpk = new CButton(L"Btn_OpenNpk");
    OpenNpk->SetDelegate(this, (DELEGATE_0)&CLevel_Edit::LoadNPKCallback);
	OpenNpk->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"LoadNPK_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\LoadNPK_Idle.animation"), BtnState::IDLE);
	OpenNpk->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"LoadNPK_CursorOn"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\LoadNPK_CursorOn.animation"), BtnState::CURSOR_ON);
	OpenNpk->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"LoadNPK_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\LoadNPK_Pressed.animation"), BtnState::PRESSED);
	AddObject(OpenNpk, LayerType::UI);
	OpenNpk->SetScale(Vec2D(219, 47));
	OpenNpk->SetLocation(Vec2D(833, 10));

	// 이미지 파일 로드 버튼 추가
	CButton* MakeAlbum = new CButton(L"Btn_MakeAlbum");
	MakeAlbum->SetDelegate(this, (DELEGATE_0)&CLevel_Edit::CreateAlbumCallback);
	MakeAlbum->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"CreateAlbum_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\CreateAlbum_Idle.animation"), BtnState::IDLE);
	MakeAlbum->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"CreateAlbum_CursorOn"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\CreateAlbum_CursorOn.animation"), BtnState::CURSOR_ON);
	MakeAlbum->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"CreateAlbum_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\CreateAlbum_Pressed.animation"), BtnState::PRESSED);
	AddObject(MakeAlbum, LayerType::UI);
	MakeAlbum->SetScale(Vec2D(219, 47));
	MakeAlbum->SetLocation(Vec2D(833, 65));

	// 애니메이션 파일 편집 버튼 추가
	CButton* EditAni = new CButton(L"Btn_MakeAlbum");
	EditAni->SetDelegate(this, (DELEGATE_0)&CLevel_Edit::EditAnimationCallback);
	EditAni->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"EditAnimation_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\EditAnimation_Idle.animation"), BtnState::IDLE);
	EditAni->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"EditAnimation_CursorOn"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\EditAnimation_CursorOn.animation"), BtnState::CURSOR_ON);
	EditAni->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"EditAnimation_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\EditAnimation_Pressed.animation"), BtnState::PRESSED);
	AddObject(EditAni, LayerType::UI);
	EditAni->SetScale(Vec2D(219, 47));
	EditAni->SetLocation(Vec2D(833, 120));


	// 에디터 레벨 BGM 세팅
	CSound* pStartBGM = CSoundMgr::GetInst()->GetSound(L"EditBGM", L"\\music\\herring_archipelago.ogg");
	pStartBGM->PlayToBGM(true);

	CLevel::Begin();
}

void CLevel_Edit::Tick()
{
	// ESC 누르면 시작레벨로
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::ESC) == Key_state::PRESSED)
	{
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"Level_Start"));
	}

	CLevel::Tick();

}

void CLevel_Edit::FinalTick()
{
	if (m_PreviewPlayer != nullptr)
		m_PreviewPlayer->FinalTick();
	CLevel::FinalTick();
}

void CLevel_Edit::Render()
{
	if (m_PreviewTexture != nullptr)
		m_PreviewTexture->DirectRender();
	if (m_PreviewPlayer != nullptr)
		m_PreviewPlayer->DirectRender();
	CLevel::Render();
}

void CLevel_Edit::End()
{
	ClearObject();
	SetPreviewPlayer(nullptr);
	if (m_hAlbumViewerWnd)
		DestroyWindow(m_hAlbumViewerWnd);
	if (m_hCreateAlbum)
		DestroyWindow(m_hCreateAlbum);
	if (m_hEditAnimation)
		DestroyWindow(m_hEditAnimation);
}

void CLevel_Edit::SetPreviewPlayer(CAlbumPlayer* _pAlbPlayer)
{
	if (m_PreviewPlayer)
	{
		delete m_PreviewPlayer;
		m_PreviewPlayer = nullptr;
	}
	m_PreviewPlayer = _pAlbPlayer;
}



//=========
// 전역함수
//=========
// 해상도 조절 메뉴 - 프레임 드랍 문제로 삭제
//bool EditorMenu(HINSTANCE _hInst, HWND _hWnd, int _wmID)
//{
//	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
//	if (!dynamic_cast<CLevel_Edit*>(pLevel))
//		return false;
//
//	switch (_wmID)
//	{
//	case ID_RES600:
//	{
//
//		CEngine::GetInst()->SetResolution(Vec2D(1066, 600));
//		CEngine::GetInst()->SetScreenScale(1);
//		CEngine::GetInst()->ChangeWindowSize(Vec2D(CEngine::GetInst()->GetResolution().x, CEngine::GetInst()->GetResolution().y));
//		CTextMgr::GetInst()->ChangeTextSize(CEngine::GetInst()->GetResolution().y / 20);
//		return true;
//	}
//	case ID_RES900:
//	{
//		CEngine::GetInst()->SetResolution(Vec2D(1600, 900));
//		CEngine::GetInst()->SetScreenScale(1.5);
//		CEngine::GetInst()->ChangeWindowSize(Vec2D(CEngine::GetInst()->GetResolution().x, CEngine::GetInst()->GetResolution().y));
//		CTextMgr::GetInst()->ChangeTextSize(CEngine::GetInst()->GetResolution().y / 20);
//		return true;
//	}
//	case ID_RES1200:
//	{
//		CEngine::GetInst()->SetResolution(Vec2D(2133, 1200));
//		CEngine::GetInst()->SetScreenScale(2);
//		CEngine::GetInst()->ChangeWindowSize(Vec2D(CEngine::GetInst()->GetResolution().x, CEngine::GetInst()->GetResolution().y));
//		CTextMgr::GetInst()->ChangeTextSize(CEngine::GetInst()->GetResolution().y / 20);
//		return true;
//	}
//	}
//	return false;
//}





void CLevel_Edit::LoadNPKCallback()
{
    if (m_hAlbumViewerWnd == nullptr)
    {
        m_hAlbumViewerWnd = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AlbumViewer), CEngine::GetInst()->GetMainWnd(), &AlbumViewerProc);
    }
    else
    {
        DestroyWindow(m_hAlbumViewerWnd);
        m_hAlbumViewerWnd = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AlbumViewer), CEngine::GetInst()->GetMainWnd(), &AlbumViewerProc);
    }

    ShowWindow(m_hAlbumViewerWnd, SW_SHOW);
}

void CLevel_Edit::CreateAlbumCallback()
{
	if (m_hCreateAlbum == nullptr)
	{
		m_hCreateAlbum = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AlbumMaker), CEngine::GetInst()->GetMainWnd(), &CreateAlbumProc);
	}
	else
	{
		DestroyWindow(m_hCreateAlbum);
		m_hCreateAlbum = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AlbumMaker), CEngine::GetInst()->GetMainWnd(), &CreateAlbumProc);
	}
	ShowWindow(m_hCreateAlbum, SW_SHOW);
}

void CLevel_Edit::EditAnimationCallback()
{
	if (m_hEditAnimation == nullptr)
	{
		m_hEditAnimation = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AnimationEditor), CEngine::GetInst()->GetMainWnd(), &EditAlimationProc);
	}
	else
	{
		DestroyWindow(m_hEditAnimation);
		m_hEditAnimation = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AnimationEditor), CEngine::GetInst()->GetMainWnd(), &EditAlimationProc);
	}
	ShowWindow(m_hEditAnimation, SW_SHOW);
}