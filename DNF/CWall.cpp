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
	Vec2D OtherGndPos = OtherObj->GetGroundPos();
	Vec2D ThisCenter = GetLocation() + (GetScale() / 2.f);
	Vec2D CenterDiff = OtherGndPos - ThisCenter;
	// �浹�Ѱ� �� �Ǵ� ��Ż�� ��� �ƹ� �ϵ� �Ͼ�� ����
	if (dynamic_cast<CWall*>(OtherObj) != nullptr)
		return;
	else if (dynamic_cast<CPortal*>(OtherObj) != nullptr)
		return;

	// �浹�� ������Ʈ�� ƨ�ܳ�
	//Vec2D ref1 = GetScale();
	//Vec2D ref2(GetScale().x, -(GetScale().y));
	//Vec2D test1(OtherObj->GetGroundPos() - GetLocation());
	//Vec2D test2(OtherObj->GetGroundPos() - (GetLocation() + Vec2D(0.f, GetScale().y)));
	//if (ref1.Cross(test1) * ref2.Cross(test2) >= 0)			// �� & �Ʒ�
	//{
	//	OtherObj->SetGroundPos(Vec2D(OtherColPos.x, ThisCenter.y + CenterDiff.y));
	//}
	//else if (ref1.Cross(test1) * ref2.Cross(test2) < 0)		// ������ & ����
	//{
	//	OtherObj->SetLocation(Vec2D(ThisCenter.x + CenterDiff.x, OtherColPos.y));
	//}

	float WallDiffX = (GetScale() / 2.f).x - abs(CenterDiff.x);
	float WallDiffY = (GetScale() / 2.f).y - abs(CenterDiff.y);
	if (WallDiffX > 0 && WallDiffY > 0)
	{
		if (WallDiffX > WallDiffY)
		{
			OtherObj->SetGroundPos(OtherObj->GetGroundPos() + Vec2D(0.f, CenterDiff.y > 0 ? WallDiffY : -WallDiffY));
		}
		else
		{
			OtherObj->SetGroundPos(OtherObj->GetGroundPos() + Vec2D(CenterDiff.x > 0 ? WallDiffX : -WallDiffX, 0.f));
		}
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
