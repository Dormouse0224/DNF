#include "pch.h"
#include "CDummy.h"

CDummy::CDummy(wstring _name)
	: CObj(_name)
{
}

CDummy::~CDummy()
{
}

void CDummy::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CDummy::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CDummy::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CDummy::Begin()
{
}

void CDummy::Tick()
{

}

void CDummy::Render()
{
	CObj::Render();
}
