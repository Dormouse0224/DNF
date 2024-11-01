#include "pch.h"
#include "CPlayer_Walk.h"
#include "CObj.h"
#include "CRigidBody.h"
#include "CCollider.h"


CPlayer_Walk::CPlayer_Walk(wstring _name)
	: CState(_name)
{
}

CPlayer_Walk::~CPlayer_Walk()
{
}

void CPlayer_Walk::Enter()
{
	GetOwnerObj()->SetState((int)PlayerState::Walk);
	GetOwnerObj()->GetBodyCollider()->SetSize(Vec2D(36, 95));
	GetOwnerObj()->GetBodyCollider()->SetOffset(Vec2D(0, 5));
}

void CPlayer_Walk::FinalTick()
{
	Vec2D curSpd = GetOwnerObj()->GetRigidBody()->GetSpeed();

	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::UP) == Key_state::NONE
		&& CKeyMgr::GetInst()->GetKeyState(Keyboard::DOWN) == Key_state::NONE
		&& CKeyMgr::GetInst()->GetKeyState(Keyboard::LEFT) == Key_state::NONE
		&& CKeyMgr::GetInst()->GetKeyState(Keyboard::RIGHT) == Key_state::NONE)
	{
		GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Idle));
	}
	else
	{
		Vec2D spd(0, 0);
		if (CKeyMgr::GetInst()->GetKeyState(Keyboard::UP) == Key_state::PRESSED)
			spd = spd - Vec2D(0.f, m_Speed);
		if (CKeyMgr::GetInst()->GetKeyState(Keyboard::DOWN) == Key_state::PRESSED)
			spd = spd + Vec2D(0.f, m_Speed);
		if (CKeyMgr::GetInst()->GetKeyState(Keyboard::LEFT) == Key_state::PRESSED)
			spd = spd - Vec2D(m_Speed, 0.f);
		if (CKeyMgr::GetInst()->GetKeyState(Keyboard::RIGHT) == Key_state::PRESSED)
			spd = spd + Vec2D(m_Speed, 0.f);

		GetOwnerObj()->GetRigidBody()->SetSpeed(spd);
	}


	if (CKeyMgr::GetInst()->GetCommand() == vector<Keyboard>{ Keyboard::LEFT, Keyboard::LEFT }
	|| CKeyMgr::GetInst()->GetCommand() == vector<Keyboard>{ Keyboard::RIGHT, Keyboard::RIGHT })
	{
		GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Run));
	}

	
	
}

void CPlayer_Walk::Exit()
{
}
