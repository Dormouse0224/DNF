#include "pch.h"
#include "CWindow.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CTextureMgr.h"
#include "CDbgRender.h"
#include "CCameraMgr.h"
#include "CAlbumPlayer.h"
#include "CTextMgr.h"

CWindow::CWindow(wstring _name)
	: CUI(_name)
	, m_WindowColor(0, 0, 0, 0)
	, m_BorderColor(0, 0, 0, 0)
	, m_PrevCursorPos(0, 0)
	, m_bMovalbe(false)
	, m_BeginCursorPos(0, 0)
	, m_EndCursorPos(0, 0)
{
}

CWindow::~CWindow()
{
}

void CWindow::Tick_UI()
{
	if (m_bMovalbe && m_IsPressed)
	{
		SetUIPos(GetUIPos() + (CKeyMgr::GetInst()->GetMousePos() - m_PrevCursorPos));
		m_PrevCursorPos = CKeyMgr::GetInst()->GetMousePos();
	}
}

void CWindow::Render_UI()
{
	CTextureMgr::GetInst()->FillRect(m_WindowColor, GetLocation(), GetScale());
	CTextureMgr::GetInst()->DrawRect(m_BorderColor, 3, GetLocation(), GetScale());
	for (int i = 0; i < m_AlbumPlayerVector.size(); ++i)
	{
		m_AlbumPlayerVector[i]->Render(this);
	}

	if (m_RenderDelegate1)
	{
		(m_DelegateObj->*m_RenderDelegate1)(m_RenderDelegate1_param);
	}

	if (!m_Text.empty())
	{
		CTextMgr::GetInst()->Render(GetLocation().x + 5, GetLocation().y + 5, m_Text, m_TextColor);
	}
}

void CWindow::MouseLBtnDown()
{
	m_PrevCursorPos = CKeyMgr::GetInst()->GetMousePos();
	m_BeginCursorPos = CKeyMgr::GetInst()->GetMousePos();
}

void CWindow::MouseReleased()
{
	m_EndCursorPos = CKeyMgr::GetInst()->GetMousePos();
}

