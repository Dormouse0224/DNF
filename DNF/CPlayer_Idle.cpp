#include "pch.h"
#include "CPlayer_Idle.h"
#include "CKeyMgr.h"
#include "CObj.h"
#include "CCollider.h"

CPlayer_Idle::CPlayer_Idle(wstring _name)
	: CState(_name)
{
}

CPlayer_Idle::~CPlayer_Idle()
{
}

void CPlayer_Idle::Enter()
{
	GetOwnerObj()->SetState((int)PlayerState::Idle);
	GetOwnerObj()->GetBodyCollider()->SetSize(Vec2D(33, 95));
	GetOwnerObj()->GetBodyCollider()->SetOffset(Vec2D(0, 5));
}

void CPlayer_Idle::FinalTick()
{
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::UP) == Key_state::TAP
		|| CKeyMgr::GetInst()->GetKeyState(Keyboard::DOWN) == Key_state::TAP
		|| CKeyMgr::GetInst()->GetKeyState(Keyboard::LEFT) == Key_state::TAP
		|| CKeyMgr::GetInst()->GetKeyState(Keyboard::RIGHT) == Key_state::TAP)
	{
		GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Walk));
	}
	if (CKeyMgr::GetInst()->GetCommand() == vector<Keyboard>{ Keyboard::LEFT, Keyboard::LEFT }
	|| CKeyMgr::GetInst()->GetCommand() == vector<Keyboard>{ Keyboard::RIGHT, Keyboard::RIGHT })
	{
		GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Run));
	}
}

void CPlayer_Idle::Exit()
{
}
