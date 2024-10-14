#include "pch.h"
#include "CButton.h"
#include "CKeyMgr.h"
#include "CAlbumPlayer.h"
#include "CTextMgr.h"

CButton::CButton(wstring _Name)
	:CUI(_Name)
	, m_Action{ nullptr, nullptr, nullptr }
{
}

CButton::~CButton()
{
	
}

void CButton::Render()
{
	if (IsCursorOn())
	{
		if (m_IsPressed)
		{
			// ��ư ���� �ִϸ��̼�
			if (m_Action[(int)BtnState::PRESSED] != nullptr)
				m_Action[(int)BtnState::PRESSED]->Render(this, false);
		}
		else
		{
			// ��ư Ŀ���� �ִϸ��̼�
			if (m_Action[(int)BtnState::CURSOR_ON] != nullptr)
				m_Action[(int)BtnState::CURSOR_ON]->Render(this, false);
		}
	}
	else
	{
		// ��ư �⺻ �ִϸ��̼�
		if (m_Action[(int)BtnState::IDLE] != nullptr)
			m_Action[(int)BtnState::IDLE]->Render(this, false);
	}
}

void CButton::SetAction(CAlbumPlayer* _Scene, BtnState _BtnState)
{
	m_Action[(int)_BtnState] = _Scene;
	AddComponent(_Scene);
}
