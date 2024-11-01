#include "pch.h"
#include "CPlayer_Run.h"
#include "CObj.h"
#include "CKeyMgr.h"
#include "CRigidBody.h"
#include "CCollider.h"

CPlayer_Run::CPlayer_Run(wstring _name)
	: CState(_name)
{
}

CPlayer_Run::~CPlayer_Run()
{
}

void CPlayer_Run::Enter()
{
	GetOwnerObj()->SetState((int)PlayerState::Run);
	GetOwnerObj()->GetBodyCollider()->SetSize(Vec2D(86, 75));
	GetOwnerObj()->GetBodyCollider()->SetOffset(Vec2D(0, 20));
}

void CPlayer_Run::FinalTick()
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
}

void CPlayer_Run::Exit()
{
}
