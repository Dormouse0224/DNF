#include "pch.h"
#include "CButton.h"
#include "CKeyMgr.h"
#include "CAlbumPlayer.h"
#include "CTextMgr.h"

CButton::CButton(wstring _Name)
	:CObj(_Name)
	, m_Action{}
	, m_CurBtnState(BtnState::Idle)
	, m_PrevBtnState(BtnState::Idle)
	, m_IsBtnReleased(false)
{
}

CButton::~CButton()
{
	for (int i = 0; i < (int)BtnState::END; ++i)
	{
		if (m_Action[i] != nullptr)
		{
			delete m_Action[i];
			m_Action[i] = nullptr;
		}
	}

	memset(m_Action, 0, sizeof(m_Action));
}

void CButton::Begin()
{
	CTextMgr::GetInst()->WriteText(m_Text[(int)m_CurBtnState]);
}

void CButton::Tick()
{
	Vec2D Cursor = CKeyMgr::GetInst()->GetMousePos();
	Vec2D LT = Vec2D(GetLocation().x - (GetScale().x / 2), GetLocation().y - (GetScale().y / 2));
	Vec2D RB = Vec2D(GetLocation().x + (GetScale().x / 2), GetLocation().y + (GetScale().y / 2));
	Key_state MLBtn = CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN);
	if (MLBtn == Key_state::TAP && (Cursor > LT && Cursor < RB))
	{
		m_PrevBtnState = m_CurBtnState;
		m_CurBtnState = BtnState::Pressed;
	}
	else if (MLBtn == Key_state::RELEASE && (Cursor > LT && Cursor < RB))
	{
		m_PrevBtnState = m_CurBtnState;
		m_CurBtnState = BtnState::Idle;
		m_IsBtnReleased = true;
	}
	else if (MLBtn == Key_state::PRESSED && (Cursor > LT && Cursor < RB))
	{
		if (m_CurBtnState == BtnState::Pressed)
		{
			m_PrevBtnState = m_CurBtnState;
			m_CurBtnState = BtnState::Pressed;
		}
	}
	else if (Cursor > LT && Cursor < RB)
	{
		m_PrevBtnState = m_CurBtnState;
		m_CurBtnState = BtnState::OnCursor;
	}
	else
	{
		m_PrevBtnState = m_CurBtnState;
		m_CurBtnState = BtnState::Idle;
	}
}

void CButton::Render()
{
	if (m_Action[(int)m_CurBtnState])
		m_Action[(int)m_CurBtnState]->Render(this);

	if (m_CurBtnState != m_PrevBtnState)
	{
		CTextMgr::GetInst()->WriteText(m_Text[(int)m_CurBtnState]);
		CTextMgr::GetInst()->DeleteText(m_Text[(int)m_PrevBtnState]);
	}

}
