#include "pch.h"
#include "CButton.h"
#include "CKeyMgr.h"
#include "CAlbumPlayer.h"
#include "CTextMgr.h"

CButton::CButton(wstring _Name)
	:CUI(_Name)
	, m_Action{ nullptr, nullptr, nullptr }
	, m_Text()
{
}

CButton::~CButton()
{
	
}

void CButton::Render_UI()
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

	if (!m_Text.empty())
	{
		CTextMgr::GetInst()->Render(GetLocation().x + 5, GetLocation().y + 1, m_Text, m_TextColor);
	}
}

void CButton::AddComponent_Btn(CAlbumPlayer* _Scene, BtnState _BtnState)
{
	m_Action[(int)_BtnState] = _Scene;
	AddComponent(_Scene);
}

void CButton::MouseLBtnClikced()
{
	if (m_ClickCallback0)
	{
		m_ClickCallback0();
	}
	if (m_ClickCallback1)
	{
		m_ClickCallback1(m_ClickCallback1_param);
	}
	
	if (m_ClickDelegate0)
	{
		(m_DelegateObj->*m_ClickDelegate0)();
	}
	if (m_ClickDelegate1)
	{
		(m_DelegateObj->*m_ClickDelegate1)(m_ClickDelegate1_param);
	}
}
