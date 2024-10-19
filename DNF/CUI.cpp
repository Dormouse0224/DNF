#include "pch.h"
#include "CUI.h"
#include "CKeyMgr.h"
#include "CEngine.h"

CUI::CUI(wstring _name)
	: CObj(_name)
	, m_UIPos(0, 0)
	, m_Parent(nullptr)
	, m_ChildVec{}
	, m_IsPressed(false)
{
}

CUI::~CUI()
{
}

void CUI::Tick()
{
	m_UIPos = GetLocation();
	if (m_Parent)
	{
		m_UIPos = m_UIPos + m_Parent->m_UIPos;
	}

	// 마우스 커서가 영역 내에 있는지 검사
	Vec2D Cursor = CKeyMgr::GetInst()->GetMousePos();
	Vec2D LT = Vec2D(GetLocation().x, GetLocation().y) * CEngine::GetInst()->GetScreenScale();
	Vec2D RB = Vec2D(GetLocation().x + GetScale().x, GetLocation().y + GetScale().y) * CEngine::GetInst()->GetScreenScale();
	if (Cursor >> LT && Cursor << RB)
	{
		m_IsCursorOn = true;
	}
	else
	{
		m_IsCursorOn = false;
	}

	// 실제 UI 가 추가적으로 할일
	Tick_UI();

	// 자식 UI Tick 호출
	for (size_t i = 0; i < m_ChildVec.size(); ++i)
	{
		m_ChildVec[i]->Tick();
	}
}

void CUI::AddChild(CUI* _Child, wstring _name)
{
	m_ChildVec.push_back(_Child);
	_Child->m_Parent = this;
}

