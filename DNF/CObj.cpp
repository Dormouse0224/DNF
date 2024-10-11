#include "pch.h"
#include "CObj.h"
#include "CEngine.h"
#include "CComponent.h"
#include "CCollider.h"

CObj::CObj(wstring _Name)
	: CBase(_Name)
	, m_Location(Vec2D{ 0, 0 })
	, m_Scale(Vec2D{ 0, 0 })
	, m_Dead(false)
{
}

CObj::CObj(wstring _Name, Vec2D _Loc, Vec2D _Scale)
	: CBase(_Name)
	, m_Location(_Loc)
	, m_Scale(_Scale)
{
}

CObj::~CObj()
{
	Delete_Vector(m_ComponentVector);
}

void CObj::AddComponent(CComponent* _Component)
{
	m_ComponentVector.push_back(_Component);
	_Component->m_Owner = this;
}

void CObj::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CObj::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CObj::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CObj::Begin()
{
}


void CObj::FinalTick()
{
	for (size_t i = 0; i < m_ComponentVector.size(); ++i)
	{
		m_ComponentVector[i]->FinalTick();
	}
}

void CObj::Render()
{
	HDC dc = CEngine::GetInst()->GetSubDC();
}

template<typename T>
T* CObj::GetComponent(wstring _ComponentName)
{
	for (int i = 0; i < m_ComponentVector.size(); ++i)
	{
		if (m_ComponentVector[i]->GetName() == _ComponentName && nullptr != dynamic_cast<T>(m_ComponentVector[i]))
		{
			return (T*)m_ComponentVector[i];
		}
	}
	return nullptr;
}