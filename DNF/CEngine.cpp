#include "pch.h"
#include "CEngine.h"

#include "CKeyMgr.h"
#include "CLevelMgr.h"
#include "CTimeMgr.h"
#include "CTaskMgr.h"
#include "CTextMgr.h"
#include "CTextureMgr.h"
#include "CCameraMgr.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"
#include "CTexture.h"
#include "CStickerMgr.h"
#include "CCollisionMgr.h"

#include "CSelectGDI.h"
#include "CDbgRender.h"
#include "resource.h"


CEngine::CEngine()
	: m_hInst(nullptr)
	, m_hMainDC(nullptr)
	, m_hMainWnd(nullptr)
	, m_Backbuffer(nullptr)
	, m_Resolution(1066.f, 600.f)
	, m_ScreenScale(1.f)
	, m_MainWndPos(0, 0)
	, m_WindowSize(0, 0)
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

CEngine::~CEngine()
{
	DeleteObject(m_hSubBitmap);
	GdiplusShutdown(gdiplusToken);
}



int CEngine::Init(HINSTANCE _hInst)
{

	WCHAR curdir[255] = {};
	GetCurrentDirectory(255, curdir);
	m_ResourcePath = curdir;
	m_ResourcePath += L"\\resource";

	ifstream DataFile;
	DataFile.open("DNF_program.data", ios::binary);
	if (!DataFile.is_open())
	{
		ofstream DataFile2;
		DataFile2.open("DNF_program.data", ios::binary);
		ProgramInfo info2;
		info2.Resolution = Vec2D(CEngine::GetInst()->GetResolution().x, CEngine::GetInst()->GetResolution().y);
		info2.WindowPos = m_MainWndPos;
		info2.Scale = CEngine::GetInst()->GetScreenScale();
		DataFile2.write((char*)&info2, sizeof(info2));
		DataFile2.close();
		DataFile.open("DNF_program.data", ios::binary);
	}
	ProgramInfo info;
	DataFile.read((char*)&info, sizeof(info));
	DataFile.close();

	m_Resolution = info.Resolution;
	m_ScreenScale = info.Scale;
	m_MainWndPos = info.WindowPos;

	// 멤버 초기화
	m_hInst = _hInst;

	m_hMainWnd = CreateWindowW(L"main_window", L"Dungeon & Fighter", (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_VISIBLE),
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, m_hInst, nullptr);

	// 윈도우 크기 변경
	ChangeWindowSize(Vec2D(m_Resolution.x, m_Resolution.y));

	if (!m_hMainWnd)
		return E_FAIL;

	// 매니저 객체 생성 및 초기화
	CKeyMgr::GetInst()->Init();
	CTextureMgr::GetInst()->Init();
	CreateBackBuffer();		// 백버퍼 생성
	CSoundMgr::GetInst()->Init();
	CLevelMgr::GetInst()->Init();
	CTimeMgr::GetInst()->Init();
	CTextMgr::GetInst()->Init();
	CCameraMgr::GetInst()->Init();


	// 메인 DC 생성
	m_hMainDC = GetDC(m_hMainWnd);


	// GDIObject 생성
	CreateGDIObject();


	return S_OK;
}

void CEngine::Progress()
{
	// 매니저 프레임 행동 수행
	CKeyMgr::GetInst()->Tick();
	CTimeMgr::GetInst()->Tick();
	CCameraMgr::GetInst()->Tick();
	CUIMgr::GetInst()->Tick();
	CStickerMgr::GetInst()->Tick();

	// 레벨 프레임 행동 수행
	CLevelMgr::GetInst()->Progress();

	// 충돌 검사
	CCollisionMgr::GetInst()->Tick();


	// 백버퍼 화면 초기화
	{
		CTextureMgr::GetInst()->FillRect(Color(255, 100, 100, 100), Vec2D(0, 0), m_Resolution, true);
	}

	// 레벨 렌더링
	CLevelMgr::GetInst()->Render();

	// 카메라 이펙트
	CCameraMgr::GetInst()->Effect();

	// 디버그 렌더링
	CDbgRender::GetInst()->Render();

	// 백버퍼 비트맵을 프런트버퍼 비트맵으로 복사
	m_Backbuffer->GetBitmap()->GetHBITMAP(Color::White, &m_hSubBitmap);
	DeleteObject(SelectObject(m_hSubDC, m_hSubBitmap));
	BitBlt(m_hMainDC, 0, 0, (int)m_Resolution.x, (int)m_Resolution.y, m_hSubDC, 0, 0, SRCCOPY);


	// 태스크 매니저에 요청된 태스크 수행
	CTaskMgr::GetInst()->Tick();
}


void CEngine::CreateBackBuffer()
{
	// 더블버퍼링용 DC 와 비트맵 생성, 기존 비트맵 삭제
	m_Backbuffer = CTextureMgr::GetInst()->CreateRectTexture(L"BackBuffer", m_Resolution, Vec2D(0, 0), Color(255, 0, 0, 0), true);
	m_hSubDC = CreateCompatibleDC(m_hMainDC);
	//m_SubBitmap = CTextureMgr::GetInst()->GetAlbum(_SysAlbum)->GetScene(L"BackBuffer")->GetBitmap();
	//SetBkMode(m_hSubDC, TRANSPARENT);
}

void CEngine::CreateGDIObject()
{
	// Pen
	m_Pen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_Pen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_Pen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

	// Brush
	m_Brush[(UINT)BRUSH_TYPE::RED] = CreateSolidBrush(RGB(255, 0, 0));
	m_Brush[(UINT)BRUSH_TYPE::GREEN] = CreateSolidBrush(RGB(0, 255, 0));
	m_Brush[(UINT)BRUSH_TYPE::BLUE] = CreateSolidBrush(RGB(0, 0, 255));
	m_Brush[(UINT)BRUSH_TYPE::GRAY] = CreateSolidBrush(RGB(100, 100, 100));
	m_Brush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
}

void CEngine::ChangeWindowSize(Vec2D _vResolution)
{
	m_Resolution = _vResolution;

	RECT rt = { 0, 0, (LONG)m_Resolution.x, (LONG)m_Resolution.y };

	// 메인윈도우가 Menu 가 있는지 확인
	HMENU hMenu = GetMenu(m_hMainWnd);

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, !!hMenu);

	bool res = SetWindowPos(m_hMainWnd, nullptr, (int)m_MainWndPos.x, (int)m_MainWndPos.y, rt.right - rt.left, rt.bottom - rt.top, 0);
	m_WindowSize = Vec2D(rt.right - rt.left, rt.bottom - rt.top);
	MINMAXINFO minMaxInfo = {};
	SendMessage(m_hMainWnd, WM_GETMINMAXINFO, 0, (LPARAM)&minMaxInfo); // 윈도우 창 강제 재조정
}

