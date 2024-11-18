#include "pch.h"
#include "CUI.h"
#include "CKeyMgr.h"
#include "CEngine.h"
#include "CDbgRender.h"
#include "CLevelMgr.h"
#include "CCameraMgr.h"

CUI::CUI(wstring _name)
	: CObj(_name)
	, m_UIPos(0, 0)
	, m_Parent(nullptr)
	, m_ChildVec{}
	, m_IsPressed(false)
	, m_UIItemType(ItemType::END)
	, m_UIInventoryType(InventoryType::END)
{
}

CUI::~CUI()
{
	for (int i = 0; i < m_ChildVec.size(); ++i)
	{
		//CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(LayerType::UI, m_ChildVec[i]->GetID());
		delete m_ChildVec[i];
	}
}

void CUI::Tick()
{
	CDbgRender::GetInst()->AddDbgRender(DbgRenderShape::Rectangle, GetLocation(), GetScale(), 0, Color(255, 0, 0));

	Key_state MLBtn = CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN);
	if (MLBtn == Key_state::NONE)
	{
		m_PrevPressed = m_IsPressed;
		m_IsPressed = false;
	}

	if (m_Parent)
	{
		SetLocation(m_UIPos + m_Parent->GetLocation());
	}
	else
	{
		SetLocation(m_UIPos + CCameraMgr::GetInst()->GetCameraPos());
	}

	// ���콺 Ŀ���� ���� ���� �ִ��� �˻�
	Vec2D Cursor = CKeyMgr::GetInst()->GetMousePos() + CCameraMgr::GetInst()->GetCameraPos();
	Vec2D LT = GetLocation();
	Vec2D RB = GetLocation() + GetScale();
	if (Cursor >> LT && Cursor << RB)
	{
		m_IsCursorOn = true;
	}
	else
	{
		m_IsCursorOn = false;
	}


	// ���� UI �� �߰������� ����
	Tick_UI();

	// �ڽ� UI Tick ȣ��
	for (size_t i = 0; i < m_ChildVec.size(); ++i)
	{
		m_ChildVec[i]->Tick();
	}
}

void CUI::Render()
{
	// UI ���� ������
	Render_UI();

	// �ڽ� UI Render ȣ��
	for (size_t i = 0; i < m_ChildVec.size(); ++i)
	{
		m_ChildVec[i]->Render();
	}
}

void CUI::Render_UI()
{
	Vec2D vPos = m_UIPos;
	Vec2D vScale = GetScale();

	CObj::Render();
}

void CUI::FinalTick()
{
	// UI ���� ������Ʈ ƽ ȣ��
	FinalTick_UI();

	// �ڽ� UI ������Ʈ ƽ ȣ��
	for (size_t i = 0; i < m_ChildVec.size(); ++i)
	{
		m_ChildVec[i]->FinalTick();
	}
}

void CUI::FinalTick_UI()
{
	CObj::FinalTick();
}

void CUI::AddChild(CUI* _Child, wstring _name)
{
	m_ChildVec.push_back(_Child);
	_Child->m_Parent = this;
}

