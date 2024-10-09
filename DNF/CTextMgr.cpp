#include "pch.h"
#include "CTextMgr.h"
#include "CEngine.h"
#include <windows.h>

CTextMgr::CTextMgr()
	: m_FontName(L"던파 연단된 칼날 Medium")
	, m_FontPath(L"D:\\repos\\DNF\\Output\\resource\\Font\\DNFForgedBlade-Medium.ttf")
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
}

void CTextMgr::Render()
{
	HFONT defaultFont = nullptr;
	HDC SubDC = CEngine::GetInst()->GetSubDC();
	for (TextInfo info : m_VecTextInfo)
	{
		HFONT hFont = CreateFont(
			info.Height, 0, 0, 0, info.Weight, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, m_FontName.c_str());

		SetTextColor(SubDC, info.Color);

		if (defaultFont == nullptr)
			defaultFont = (HFONT)SelectObject(SubDC, hFont);

		TextOut(SubDC, info.x, info.y, info.Text.c_str(), info.Text.size());
	}
	if (defaultFont)
		SelectObject(SubDC, defaultFont);
	SetTextColor(SubDC, RGB(0, 0, 0));
}

void CTextMgr::WriteText(int _Height, int _Weight, UINT _x, UINT _y, wstring _Text, COLORREF _Color)
{
	TextInfo info;
	info.Height = _Height;
	info.Weight = _Weight;
	info.x = _x;
	info.y = _y;
	info.Text = _Text;
	info.Color = _Color;
	m_VecTextInfo.push_back(info);
}

void CTextMgr::DeleteText(UINT _x, UINT _y, wstring _Text)
{
	for (vector<TextInfo>::iterator iter = m_VecTextInfo.begin(); iter != m_VecTextInfo.end(); ++iter)
	{
		if (iter->x == _x && iter->y == _y && iter->Text == _Text)
		{
			m_VecTextInfo.erase(iter);
			break;
		}
	}
}

