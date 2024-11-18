#include "pch.h"
#include "CPlayer_Jump.h"
#include "CObj.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CKeyMgr.h"
#include "CPlayer_Attack.h"
#include "CPlayer.h"

CPlayer_Jump::CPlayer_Jump(wstring _name)
	: CState(_name)
{
}

CPlayer_Jump::~CPlayer_Jump()
{
}

void CPlayer_Jump::Enter()
{
	((CPlayer*)GetOwnerObj())->StateAnimationInit(PlayerState::Jump);
	GetOwnerObj()->SetState((int)PlayerState::Jump);
	GetOwnerObj()->GetBodyCollider()->SetSize(Vec2D(35, 80));
	GetOwnerObj()->GetBodyCollider()->SetOffset(Vec2D(10, 5));
}

void CPlayer_Jump::FinalTick()
{
	Vec2D spd(0, 0);
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::UP) == Key_state::NONE
		&& CKeyMgr::GetInst()->GetKeyState(Keyboard::DOWN) == Key_state::NONE
		&& CKeyMgr::GetInst()->GetKeyState(Keyboard::LEFT) == Key_state::NONE
		&& CKeyMgr::GetInst()->GetKeyState(Keyboard::RIGHT) == Key_state::NONE)
	{
		GetOwnerObj()->GetRigidBody()->SetSpeed(spd);
	}
	else
	{
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

	if (!(GetOwnerObj()->GetRigidBody()->GetAirborne()))
	{
		if (GetFSM()->GetPrevState() == GetFSM()->FindState((int)PlayerState::Attack))
			GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Attack));
		else
			GetFSM()->ChangeState(GetFSM()->FindState((int)PlayerState::Idle));
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

void CPlayer_Jump::Exit()
{
}
