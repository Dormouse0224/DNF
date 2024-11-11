#include "pch.h"
#include "CPlayer_Walk.h"
#include "CObj.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CPlayer_Attack.h"


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
		if (GetFSM()->GetPrevState() == GetFSM()->FindState((int)PlayerState::Attack))
			GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Attack));
		else
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

	if (GetOwnerObj()->GetRigidBody()->GetAirborne())
	{
		GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Jump));
	}

	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::SPACE) == Key_state::TAP)
	{
		GetOwnerObj()->GetRigidBody()->Jump(800.f);
	}
	
	CState* pState = GetFSM()->FindState((int)PlayerState::Attack);
	if (pState)
	{
		if (((CPlayer_Attack*)pState)->AttackCheck())
			GetFSM()->ChangeState(pState);
	}

	if (GetOwnerObj()->GetCurHP() < 0)
	{
		GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Death));
	}
}

void CPlayer_Walk::Exit()
{
}
