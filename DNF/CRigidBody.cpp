#include "pch.h"
#include "CRigidBody.h"
#include "CObj.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"

CRigidBody::CRigidBody(wstring _name)
	: CComponent(_name)
	, m_Speed(0, 0)
	, m_IsAirborne(false)
	, m_AirborneHeight(0)
	, m_AirborneSpeed(0)
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::FinalTick()
{
	Vec2D spd(0, 0);

	if (m_IsAirborne == true)
	{
		m_AirborneSpeed = m_AirborneSpeed +CLevelMgr::GetInst()->GetCurrentLevel()->GetGravity() * CTimeMgr::GetInst()->GetDeltaTime();
		m_AirborneHeight += m_AirborneSpeed * CTimeMgr::GetInst()->GetDeltaTime();
		if (m_AirborneHeight > 0)
		{
			m_IsAirborne = false;
			m_AirborneSpeed = (m_AirborneSpeed * CTimeMgr::GetInst()->GetDeltaTime() - m_AirborneHeight) / CTimeMgr::GetInst()->GetDeltaTime();
		}
	}
	else
	{
		m_AirborneHeight = 0;
		m_AirborneSpeed = 0;
	}

	spd = m_Speed + Vec2D(0.f, m_AirborneSpeed);

	GetOwner()->SetLocation(GetOwner()->GetLocation() + spd * CTimeMgr::GetInst()->GetDeltaTime());
}

void CRigidBody::Jump(float _power)
{
	m_IsAirborne = true;
	m_AirborneSpeed = -_power;
}
