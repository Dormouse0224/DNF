#include "pch.h"
#include "CUI.h"
#include "CKeyMgr.h"
#include "CEngine.h"

CUI::CUI(wstring _name)
	: CObj(_name)
{
}

CUI::~CUI()
{
}

void CUI::Tick()
{
	Vec2D Cursor = CKeyMgr::GetInst()->GetMousePos();
	Vec2D LT = Vec2D(GetLocation().x, GetLocation().y) * CEngine::GetInst()->GetScreenScale();
	Vec2D RB = Vec2D(GetLocation().x + GetScale().x, GetLocation().y + GetScale().y) * CEngine::GetInst()->GetScreenScale();
	if (Cursor > LT && Cursor < RB)
	{
		m_IsCursorOn = true;
	}
	else
	{
		m_IsCursorOn = false;
	}
}

