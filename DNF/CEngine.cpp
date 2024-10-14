#include "pch.h"
#include "CEngine.h"

#include "CKeyMgr.h"
#include "CLevelMgr.h"
#include "CTimeMgr.h"
#include "CTaskMgr.h"
#include "CTextMgr.h"
#include "CTextureMgr.h"
#include "CCameraMgr.h"

#include "CSelectGDI.h"
#include "CDbgRender.h"
#include "resource.h"

CEngine::CEngine()
	: m_hInst(nullptr)
	, m_hMainDC(nullptr)
	, m_hMainWnd(nullptr)
	, m_hSubBitmap(nullptr)
	, m_hSubDC(nullptr)
	, m_Resolution(1066.f, 600.f)
	, m_ScreenScale(1.f)
	, m_MainWndPos(Vec2D(0, 0))
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

CEngine::~CEngine()
{
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

	// ��� �ʱ�ȭ
	m_hInst = _hInst;

	m_hMainWnd = CreateWindowW(L"main_window", L"Dungeon & Fighter", (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_VISIBLE),
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, m_hInst, nullptr);

	// ������ ũ�� ����
	ChangeWindowSize(Vec2D(m_Resolution.x, m_Resolution.y));

	if (!m_hMainWnd)
		return E_FAIL;

	// �Ŵ��� ��ü ���� �� �ʱ�ȭ
	CKeyMgr::GetInst()->Init();
	CTextureMgr::GetInst()->Init();
	CreateBackBuffer();		// ����� ����
	CLevelMgr::GetInst()->Init();
	CTimeMgr::GetInst()->Init();
	CTextMgr::GetInst()->Init();
	CCameraMgr::GetInst()->Init();


	// ���� DC ����
	m_hMainDC = GetDC(m_hMainWnd);


	// GDIObject ����
	CreateGDIObject();


	return S_OK;
}

void CEngine::Progress()
{
	// �Ŵ��� ������ �ൿ ����
	CKeyMgr::GetInst()->Tick();
	CTimeMgr::GetInst()->Tick();
	CCameraMgr::GetInst()->Tick();

	// ���� ������ �ൿ ����
	CLevelMgr::GetInst()->Progress();


	// ����� ȭ�� �ʱ�ȭ
	{
		SELECT_BRUSH(BRUSH_TYPE::GRAY);
		Rectangle(m_hSubDC, -1, -1, (int)m_Resolution.x + 1, (int)m_Resolution.y + 1);
	}
	// ���� ������
	CLevelMgr::GetInst()->Render();
	// �ؽ�Ʈ ������
	CTextMgr::GetInst()->Render();
	// ����� ������
	CDbgRender::GetInst()->Render();
	// ����� ��Ʈ���� ����Ʈ���� ��Ʈ������ ����
	BitBlt(m_hMainDC, 0, 0, (int)m_Resolution.x, (int)m_Resolution.y, m_hSubDC, 0, 0, SRCCOPY);



	// �½�ũ �Ŵ����� ��û�� �½�ũ ����
	CTaskMgr::GetInst()->Tick();
}


void CEngine::CreateBackBuffer()
{
	// ������۸��� DC �� ��Ʈ�� ����, ���� ��Ʈ�� ����
	//m_hSubBitmap = CreateCompatibleBitmap(m_hMainDC, (int)m_Resolution.x, (int)m_Resolution.y);
	m_hSubDC = CTextureMgr::GetInst()->CreateRectTexture(L"BackBuffer", m_Resolution, Vec2D(0, 0), Color(255, 0, 0, 0), true);
	SetBkMode(m_hSubDC, TRANSPARENT);
	//m_hSubDC = CreateCompatibleDC(m_hMainDC);
	
	//DeleteObject(SelectObject(m_hSubDC, m_hSubBitmap));
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

	// ���������찡 Menu �� �ִ��� Ȯ��
	HMENU hMenu = GetMenu(m_hMainWnd);

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, !!hMenu);

	SetWindowPos(m_hMainWnd, nullptr, m_MainWndPos.x, m_MainWndPos.y, rt.right - rt.left, rt.bottom - rt.top, 0);

	// �� �ػ��� ��Ʈ�� �ۼ� �� ����ۿ� ����, ���� ��Ʈ�� ����
	m_hSubBitmap = CreateCompatibleBitmap(m_hMainDC, (int)m_Resolution.x, (int)m_Resolution.y);
	DeleteObject(SelectObject(m_hSubDC, m_hSubBitmap));
}

