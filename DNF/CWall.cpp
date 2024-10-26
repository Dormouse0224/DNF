#include "pch.h"
#include "CWall.h"
#include "CCollider.h"
#include "CPortal.h"

CWall::CWall(wstring _name)
	: CObj(_name)
{
}

CWall::~CWall()
{
}

void CWall::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
	CObj* OtherObj = _Other->GetOwner();
	Vec2D OtherPos = OtherObj->GetLocation();
	Vec2D ThisCenter = GetLocation() + (GetScale() / 2.f);
	Vec2D CenterDiff = (OtherPos + (_Other->GetOwner()->GetScale() / 2.f)) - ThisCenter;
	// 충돌한게 벽 또는 포탈일 경우 아무 일도 일어나지 않음
	if (dynamic_cast<CWall*>(OtherObj) != nullptr)
		return;
	else if (dynamic_cast<CPortal*>(OtherObj) != nullptr)
		return;

	// 충돌한 오브젝트를 튕겨냄
	Vec2D ref1 = GetScale();
	Vec2D ref2(GetScale().x, -(GetScale().y));
	Vec2D test(OtherObj->GetLocation() + (OtherObj->GetScale() / 2.f) - GetLocation());
	if (ref1.Cross(test) * ref2.Cross(test) >= 0)			// 위 & 아래
	{
		OtherObj->SetLocation(Vec2D(OtherPos.x, ThisCenter.y + CenterDiff.y));
	}
	else if (ref1.Cross(test) * ref2.Cross(test) < 0)		// 오른쪽 & 왼쪽
	{
		OtherObj->SetLocation(Vec2D(ThisCenter.x + CenterDiff.x, OtherPos.y));
	}
}

void CWall::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CWall::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CWall::Begin()
{
}

void CWall::Tick()
{
}

void CWall::Render()
{
}
