#include "pch.h"
#include "CSticker.h"
#include "CStageMaker.h"
#include "CLevelMgr.h"
#include "CKeyMgr.h"
#include "CCameraMgr.h"

CSticker::CSticker(wstring _name)
	:CComponent(_name)
	, m_bCursorOn(false)
{
}

CSticker::~CSticker()
{
}

void CSticker::FinalTick()
{
	Vec2D Cursor = CKeyMgr::GetInst()->GetMousePos();
	Vec2D LT = GetOwner()->GetLocation() - CCameraMgr::GetInst()->GetCameraPos();
	Vec2D RB = GetOwner()->GetLocation() + GetOwner()->GetScale() - CCameraMgr::GetInst()->GetCameraPos();
	if (Cursor >> LT && Cursor << RB)
	{
		m_bCursorOn = true;
	}
	else
	{
		m_bCursorOn = false;
	}
}

void CSticker::EraseFromLevelVec()
{
	CLevelMgr::GetInst()->GetCurrentLevel()->EraseSticker(GetID());
}
