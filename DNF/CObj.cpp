#include "pch.h"
#include "CObj.h"
#include "CEngine.h"
#include "CComponent.h"
#include "CCollider.h"
#include "CAlbumPlayer.h"
#include "CLevelMgr.h"
#include "CSticker.h"
#include "CRigidBody.h"
#include "CTextMgr.h"

CObj::CObj(wstring _Name)
	: CBase(_Name)
	, m_Location(Vec2D{ 0, 0 })
	, m_Scale(Vec2D{ 0, 0 })
	, m_Dead(false)
	, m_GroundPos(0,0)
	, m_State(0)
	, m_BodyCollider(nullptr)
	, m_RigidBody(nullptr)
	, m_bLookLeft(false)
	, m_GroundPosInit(false)
	, m_bFallowCam(false)
{
}

CObj::CObj(wstring _Name, Vec2D _Loc, Vec2D _Scale)
	: CBase(_Name)
	, m_Location(_Loc)
	, m_Scale(_Scale)
	, m_Dead(false)
	, m_GroundPos(0, 0)
	, m_State(0)
	, m_BodyCollider(nullptr)
	, m_RigidBody(nullptr)
	, m_bLookLeft(false)
	, m_GroundPosInit(false)
	, m_bFallowCam(false)
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

void CObj::SetLocation(Vec2D _Location)
{
	m_Location = _Location; 
	if (!m_GroundPosInit)
	{
		m_GroundPos = Vec2D(m_Location.x + (m_Scale.x / 2.f), m_Location.y + m_Scale.y);
		m_GroundPosInit = true;
	}
}

void CObj::SetGroundPos(Vec2D _Location)
{
	Vec2D gp((GetScale().x / 2.f), GetScale().y);
	m_GroundPos = _Location;
	if (m_RigidBody)
	{
		SetLocation(_Location + Vec2D(0.f, m_RigidBody->GetAirborneHeight()) - gp);
	}
	else
	{
		SetLocation(_Location - gp);
	}
}

void CObj::SetScale(Vec2D _Scale)
{
	m_Scale = _Scale;
	m_GroundPos = Vec2D(m_Location.x + (m_Scale.x / 2.f), m_Location.y + m_Scale.y);
	if (m_BodyCollider)
	{
		m_BodyCollider->SetSize(m_Scale);
	}
}

void CObj::GiveDamage(int _dmg)
{
	m_CurHP -= _dmg;
}

void CObj::UseMana(int _mana)
{
	m_CurMP -= _mana;
}

void CObj::AddComponent(CComponent* _Component)
{
	m_ComponentVector.push_back(_Component);
	_Component->m_Owner = this;
	// 추가되는 컴포넌트가 앨범플레이어인 경우
	if (dynamic_cast<CAlbumPlayer*>(_Component))
		m_AlbumPlayerVector.push_back((CAlbumPlayer*)_Component);
}

void CObj::RegisterBodyCollider(CCollider* _Coll)
{
	m_BodyCollider = _Coll; m_BodyCollider->SetSize(m_Scale);
}

void CObj::SetRigidBody(CRigidBody* _RB)
{
	m_RigidBody = _RB;
	AddComponent(_RB);
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

struct ThreadParams {
	CAlbumPlayer* param1;
	CObj* param2;
};

void CObj::Render()
{
	for (int i = 0; i < m_AlbumPlayerVector.size(); ++i)
	{
		m_AlbumPlayerVector[i]->Render(this, m_bFallowCam, m_bLookLeft);
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