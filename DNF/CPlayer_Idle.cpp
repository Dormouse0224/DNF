#include "pch.h"
#include "CPlayer_Idle.h"
#include "CKeyMgr.h"
#include "CObj.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CPlayer_Attack.h"
#include "CPlayer.h"

CPlayer_Idle::CPlayer_Idle(wstring _name)
	: CState(_name)
{
}

CPlayer_Idle::~CPlayer_Idle()
{
}

void CPlayer_Idle::Enter()
{
	((CPlayer*)GetOwnerObj())->StateAnimationInit(PlayerState::Idle);
	GetOwnerObj()->SetState((int)PlayerState::Idle);
	GetOwnerObj()->GetBodyCollider()->SetSize(Vec2D(33, 95));
	GetOwnerObj()->GetBodyCollider()->SetOffset(Vec2D(0, 5));
	GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));
}

void CPlayer_Idle::FinalTick()
{
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::UP) == Key_state::PRESSED
		|| CKeyMgr::GetInst()->GetKeyState(Keyboard::DOWN) == Key_state::PRESSED
		|| CKeyMgr::GetInst()->GetKeyState(Keyboard::LEFT) == Key_state::PRESSED
		|| CKeyMgr::GetInst()->GetKeyState(Keyboard::RIGHT) == Key_state::PRESSED)
	{
		GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Walk));
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

void CPlayer_Idle::Exit()
{
}
