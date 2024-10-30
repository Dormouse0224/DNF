#include "pch.h"
#include "CWall.h"
#include "CCollider.h"
#include "CPortal.h"

CWall::CWall(wstring _name)
	: CObj(_name)
{
	SetLayerType(LayerType::Object);
}

CWall::~CWall()
{
}

void CWall::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
	
}

void CWall::Overlap(CCollider* _Self, CCollider* _Other)
{
	CObj* OtherObj = _Other->GetOwner();
	Vec2D OtherColPos = _Other->GetLocation();
	Vec2D ThisCenter = GetLocation() + (GetScale() / 2.f);
	Vec2D CenterDiff = (OtherColPos + (_Other->GetSize() / 2.f)) - ThisCenter;
	// �浹�Ѱ� �� �Ǵ� ��Ż�� ��� �ƹ� �ϵ� �Ͼ�� ����
	if (dynamic_cast<CWall*>(OtherObj) != nullptr)
		return;
	else if (dynamic_cast<CPortal*>(OtherObj) != nullptr)
		return;

	// �浹�� ������Ʈ�� ƨ�ܳ�
	Vec2D ref1 = GetScale();
	Vec2D ref2(GetScale().x, -(GetScale().y));
	Vec2D test1(_Other->GetLocation() + (_Other->GetSize() / 2.f) - GetLocation());
	Vec2D test2(_Other->GetLocation() + (_Other->GetSize() / 2.f) - GetLocation() - Vec2D(GetScale().x, 0.f));
	if (ref1.Cross(test1) * ref2.Cross(test2) >= 0)			// �� & �Ʒ�
	{
		OtherObj->SetLocation(Vec2D(OtherColPos.x, ThisCenter.y + CenterDiff.y));
	}
	else if (ref1.Cross(test1) * ref2.Cross(test2) < 0)		// ������ & ����
	{
		OtherObj->SetLocation(Vec2D(ThisCenter.x + CenterDiff.x, OtherColPos.y));
	}
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
