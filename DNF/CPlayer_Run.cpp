#include "pch.h"
#include "CPlayer_Run.h"
#include "CObj.h"
#include "CKeyMgr.h"
#include "CRigidBody.h"
#include "CCollider.h"

CPlayer_Run::CPlayer_Run(wstring _name)
	: CState(_name)
	, m_bHorInit(false)
	, m_bVertInit(false)
{
}

CPlayer_Run::~CPlayer_Run()
{
}

void CPlayer_Run::Enter()
{
	GetOwnerObj()->SetState((int)PlayerState::Run);
	GetOwnerObj()->GetBodyCollider()->SetSize(Vec2D(86, 75));
}

void CPlayer_Run::FinalTick()
{
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::UP) == Key_state::NONE
		&& CKeyMgr::GetInst()->GetKeyState(Keyboard::DOWN) == Key_state::NONE
		&& CKeyMgr::GetInst()->GetKeyState(Keyboard::LEFT) == Key_state::NONE
		&& CKeyMgr::GetInst()->GetKeyState(Keyboard::RIGHT) == Key_state::NONE)
	{
		GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Idle));
	}


	CKeyMgr* keyMgr = CKeyMgr::GetInst();

	if (!m_bHorInit)
	{
		if (keyMgr->GetKeyState(Keyboard::UP) == Key_state::PRESSED)
		{
			GetOwnerObj()->GetRigidBody()->AddSpeed(Vec2D(0, -m_Speed));
			m_bHorInit = true;
		}
		else if (keyMgr->GetKeyState(Keyboard::DOWN) == Key_state::PRESSED)
		{
			GetOwnerObj()->GetRigidBody()->AddSpeed(Vec2D(0, m_Speed));
			m_bHorInit = true;
		}
	}
	else
	{
		if (keyMgr->GetKeyState(Keyboard::DOWN) == Key_state::RELEASE)
		{
			GetOwnerObj()->GetRigidBody()->AddSpeed(Vec2D(0, -m_Speed));
			m_bHorInit = false;
		}
		else if (keyMgr->GetKeyState(Keyboard::UP) == Key_state::RELEASE)
		{
			GetOwnerObj()->GetRigidBody()->AddSpeed(Vec2D(0, m_Speed));
			m_bHorInit = false;
		}
	}

	if (!m_bVertInit)
	{
		if (keyMgr->GetKeyState(Keyboard::LEFT) == Key_state::PRESSED)
		{
			GetOwnerObj()->GetRigidBody()->AddSpeed(Vec2D(-m_Speed, 0));
			m_bVertInit = true;
		}
		else if (keyMgr->GetKeyState(Keyboard::RIGHT) == Key_state::PRESSED)
		{
			GetOwnerObj()->GetRigidBody()->AddSpeed(Vec2D(m_Speed, 0));
			m_bVertInit = true;
		}
	}
	else
	{
		if (keyMgr->GetKeyState(Keyboard::RIGHT) == Key_state::RELEASE)
		{
			GetOwnerObj()->GetRigidBody()->AddSpeed(Vec2D(-m_Speed, 0));
			m_bVertInit = false;
		}
		else if (keyMgr->GetKeyState(Keyboard::LEFT) == Key_state::RELEASE)
		{
			GetOwnerObj()->GetRigidBody()->AddSpeed(Vec2D(m_Speed, 0));
			m_bVertInit = false;
		}
	}
}

void CPlayer_Run::Exit()
{
}
