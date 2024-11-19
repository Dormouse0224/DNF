#include "pch.h"
#include "CTextMgr.h"
#include "CEngine.h"
#include "CCameraMgr.h"
#include "CTexture.h"
#include <windows.h>

CTextMgr::CTextMgr()
	: m_FontName(L"���� ���ܵ� Į�� Medium")
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
	// ��Ʈ�� �ý��ۿ� �߰�
	AddFontResourceEx(m_FontPath.c_str(), FR_PRIVATE, 0);
	Font* font = new Font(L"���� ���ܵ� Į�� Medium", 20, FontStyleRegular, UnitPixel);
	m_Font = font;

	HDC hdc = GetDC(NULL);
	Graphics graphics(hdc);
	FontFamily fontFamily;
	font->GetFamily(&fontFamily);

	// ��Ʈ�� line spacing ���� ������ (�⺻ ������ em-height ����)
	int lineSpacing = fontFamily.GetLineSpacing(font->GetStyle());
	int emHeight = fontFamily.GetEmHeight(font->GetStyle());

	// Graphics�� DPI ������ ������
	float dpi = graphics.GetDpiY();

	// line spacing�� �ȼ��� ��ȯ
	m_FontHeight = static_cast<int>((font->GetSize() * lineSpacing / emHeight) * dpi / 72);
}

void CTextMgr::Render(UINT _x, UINT _y, wstring _Text, Color _color, bool bCameraFallow)
{
	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	Graphics graphics(CEngine::GetInst()->GetBackbuffer()->GetBitmap());
	if (bCameraFallow)
		CameraPos = Vec2D(0, 0);
	SolidBrush brush(_color);
	graphics.DrawString(_Text.c_str(), (INT)_Text.size(), m_Font, PointF(_x - CameraPos.x, _y - CameraPos.y), &brush);
}

