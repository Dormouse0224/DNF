#include "pch.h"
#include "CTextMgr.h"
#include "CEngine.h"
#include <windows.h>

CTextMgr::CTextMgr()
	: m_FontName(L"던파 연단된 칼날 Medium")
	, m_FontPath(CEngine::GetInst()->GetResourcePath() + L"\\Font\\DNFForgedBlade-Medium.ttf")
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
	ChangeTextSize(CEngine::GetInst()->GetResolution().y / 20);
	//m_Font = CreateFont(
	//	CEngine::GetInst()->GetResolution().y / 20, 0, 0, 0, 0, FALSE, FALSE, FALSE,
	//	DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	//	DEFAULT_PITCH | FF_DONTCARE, m_FontName.c_str());

	//DeleteObject((HFONT)SelectObject(CEngine::GetInst()->GetSubDC(), m_Font));
}

void CTextMgr::Render()
{
	for (TextInfo info : m_VecTextInfo)
	{
		SetTextColor(CEngine::GetInst()->GetSubDC(), info.Color);
		TextOut(CEngine::GetInst()->GetSubDC()
			, info.x * CEngine::GetInst()->GetScreenScale(), info.y * CEngine::GetInst()->GetScreenScale()
			, info.Text.c_str(), info.Text.size());
	}
}

void CTextMgr::WriteText(UINT _x, UINT _y, wstring _Text, COLORREF _Color)
{
	TextInfo info;
	info.x = _x;
	info.y = _y;
	info.Text = _Text;
	info.Color = _Color;
	m_VecTextInfo.push_back(info);
}

void CTextMgr::WriteText(TextInfo _info)
{
	m_VecTextInfo.push_back(_info);
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

void CTextMgr::DeleteText(TextInfo _info)
{
	for (vector<TextInfo>::iterator iter = m_VecTextInfo.begin(); iter != m_VecTextInfo.end(); ++iter)
	{
		if (iter->x == _info.x && iter->y == _info.y && iter->Text == _info.Text && iter->Color == _info.Color)
		{
			m_VecTextInfo.erase(iter);
			break;
		}
	}
}

void CTextMgr::ChangeTextSize(int _size)
{
	m_Font = CreateFont(
		_size, 0, 0, 0, 0, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, m_FontName.c_str());

	DeleteObject((HFONT)SelectObject(CEngine::GetInst()->GetSubDC(), m_Font));
}

