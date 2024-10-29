#include "pch.h"
#include "CRigidBody.h"
#include "CObj.h"
#include "CTimeMgr.h"

CRigidBody::CRigidBody(wstring _name)
	: CComponent(_name)
	, m_Speed(0, 0)
	, m_IsAirborne(false)
	, m_AirborneTime(0)
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::FinalTick()
{
	if (m_IsAirborne == true && m_AirborneTime > 0.f)
	{
		m_Speed = m_Speed + Vec2D(0.f, m_Gravity);
		m_AirborneTime -= CTimeMgr::GetInst()->GetDeltaTime();
	}
	else
	{
		m_IsAirborne = false;
		m_AirborneTime = 0.f;
	}

	GetOwner()->SetLocation(GetOwner()->GetLocation() + m_Speed * CTimeMgr::GetInst()->GetDeltaTime());
}

void CRigidBody::Jump(float _power)
{
	m_IsAirborne = true;
	AddSpeed(Vec2D(0.f, -_power));
	m_AirborneTime = _power * 2.f / m_Gravity;
}
