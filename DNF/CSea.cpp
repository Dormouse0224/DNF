#include "pch.h"
#include "CSea.h"
#include "CRigidBody.h"

CSea::CSea(wstring _name)
	: CObj(_name)
{
}

CSea::~CSea()
{
}

void CSea::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CSea::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CSea::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CSea::Begin()
{
}

void CSea::Tick()
{
	// 바닥 위치 갱신
	Vec2D gp(GetLocation().x + (GetScale().x / 2.f), GetLocation().y + GetScale().y);
	if (GetRigidBody() != nullptr)
	{
		RenewGroundPos(gp - Vec2D(0.f, GetRigidBody()->GetAirborneHeight()));
	}
	else
	{
		RenewGroundPos(gp);
	}
}

void CSea::Render()
{
	CObj::Render();
}
