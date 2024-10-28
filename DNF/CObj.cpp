#include "pch.h"
#include "CObj.h"
#include "CEngine.h"
#include "CComponent.h"
#include "CCollider.h"
#include "CAlbumPlayer.h"
#include "CLevelMgr.h"
#include "CSticker.h"

CObj::CObj(wstring _Name)
	: CBase(_Name)
	, m_Location(Vec2D{ 0, 0 })
	, m_Scale(Vec2D{ 0, 0 })
	, m_Dead(false)
	, m_GroundPos(0,0)
	, m_State(0)
{
}

CObj::CObj(wstring _Name, Vec2D _Loc, Vec2D _Scale)
	: CBase(_Name)
	, m_Location(_Loc)
	, m_Scale(_Scale)
	, m_Dead(false)
{
}

CObj::~CObj()
{
	for (int i = 0; i < m_ComponentVector.size(); ++i)
	{
		m_ComponentVector[i]->EraseFromLevelVec();
		delete m_ComponentVector[i];
	}
	m_ComponentVector.clear();
}

void CObj::AddComponent(CComponent* _Component)
{
	m_ComponentVector.push_back(_Component);
	_Component->m_Owner = this;
	// 추가되는 컴포넌트가 앨범플레이어인 경우
	if (dynamic_cast<CAlbumPlayer*>(_Component))
		m_AlbumPlayerVector.push_back((CAlbumPlayer*)_Component);
	// 충돌체인경우 충돌체를 현재 레벨의 충돌체 벡터에 등록
	else if (dynamic_cast<CCollider*>(_Component))
		CLevelMgr::GetInst()->GetCurrentLevel()->AddCollider((CCollider*)_Component, m_LayerType);
	else if (dynamic_cast<CSticker*>(_Component))
		CLevelMgr::GetInst()->GetCurrentLevel()->AddSticker((CSticker*)_Component);
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
	for (int i = 0; i < m_AlbumPlayerVector.size(); ++i)
	{
		m_AlbumPlayerVector[i]->Render(this);
	}
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