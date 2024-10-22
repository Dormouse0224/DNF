#include "pch.h"
#include "CTextMgr.h"
#include "CEngine.h"
#include "CCameraMgr.h"
#include <windows.h>

CTextMgr::CTextMgr()
	: m_FontName(L"던파 연단된 칼날 Medium")
	, m_FontPath(CEngine::GetInst()->GetResourcePathW() + L"\\Font\\DNFForgedBlade-Medium.ttf")
	, m_Font(nullptr)
{
}

CTextMgr::~CTextMgr()
{
	RemoveFontResourceEx(m_FontPath.c_str(), FR_PRIVATE, 0);
}

void CTextMgr::Init()
{
	// 폰트를 시스템에 추가
	AddFontResourceEx(m_FontPath.c_str(), FR_PRIVATE, 0);
	Font* font = new Font(L"던파 연단된 칼날 Medium", 20, FontStyleRegular, UnitPixel);
	m_Font = font;
}

void CTextMgr::Render(UINT _x, UINT _y, wstring _Text, Color _color, bool bCameraFallow)
{
	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	Graphics graphics(CEngine::GetInst()->GetSubDC());
	if (bCameraFallow)
		CameraPos = Vec2D(0, 0);
	SolidBrush brush(_color);
	graphics.DrawString(_Text.c_str(), (INT)_Text.size(), m_Font, PointF(_x - CameraPos.x, _y - CameraPos.y), &brush);
}

