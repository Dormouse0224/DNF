#include "pch.h"
#include "CButton.h"
#include "CKeyMgr.h"
#include "CAlbumPlayer.h"
#include "CTextMgr.h"

CButton::CButton(wstring _Name)
	:CUI(_Name)
	, m_Action{ nullptr, nullptr, nullptr }
	, m_IsPressed(false)
	, m_IsClicked(false)
{
}

CButton::~CButton()
{
	
}

void CButton::Begin()
{

}

void CButton::Tick()
{
	CUI::Tick();
	Key_state MLBtn = CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN);

	if (MLBtn == Key_state::NONE)
	{
		m_IsPressed = false;
	}
	else
	{
		if (MLBtn == Key_state::TAP && IsCursorOn())
		{
			m_IsPressed = true;
		}
		else if (MLBtn == Key_state::RELEASE && IsCursorOn() && m_IsPressed)
		{
			m_IsClicked = true;
			m_IsPressed = false;
		}
	}
}

void CButton::Render()
{
	if (IsCursorOn())
	{
		if (m_IsPressed)
		{
			// 버튼 눌림 애니메이션
			if (m_Action[(int)BtnState::PRESSED] != nullptr)
				m_Action[(int)BtnState::PRESSED]->Render(this);
		}
		else
		{
			// 버튼 커서온 애니메이션
			if (m_Action[(int)BtnState::CURSOR_ON] != nullptr)
				m_Action[(int)BtnState::CURSOR_ON]->Render(this);
		}
	}
	else
	{
		// 버튼 기본 애니메이션
		if (m_Action[(int)BtnState::IDLE] != nullptr)
			m_Action[(int)BtnState::IDLE]->Render(this);
	}
}

void CButton::SetAction(CAlbumPlayer* _Scene, BtnState _BtnState)
{
	m_Action[(int)_BtnState] = _Scene;
	AddComponent(_Scene);
}
