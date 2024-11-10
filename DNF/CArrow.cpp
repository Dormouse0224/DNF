#include "pch.h"
#include "CArrow.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CDbgRender.h"
#include "CMonster.h"
#include "CLargo.h"
#include "CLevelMgr.h"
#include "CTimeMgr.h"
#include "CSound.h"

CArrow::CArrow(wstring _name, Vec2D _StartSpd, float _StartHeight, float _Lifetime)
	: CObj(_name)
	, m_IdleAni(nullptr)
	, m_DeadAni{}
	, m_DeadAniG{}
	, m_bFin(false)
	, m_ATK(0)
	, m_PierceCount(1)
	, m_Lifetime(_Lifetime)
	, m_DeadSound(nullptr)
	, m_bSoundPlayed(false)
	, m_DeadFuncObj(nullptr)
	, m_DeadFunc(nullptr)
{


	CRigidBody* pRB = new CRigidBody(L"Arrow_RB");
	SetRigidBody(pRB);
	pRB->SetAirborne(true);
	pRB->SetAirborneHeight(_StartHeight);
	pRB->SetSpeed(_StartSpd);

	CCollider* pCol = new CCollider(L"Arrow_Col");
	RegisterBodyCollider(pCol);
	pCol->SetSize(Vec2D(17, 17));
	pCol->SetOffset(Vec2D(-2, -8));
	AddComponent(pCol);
}

CArrow::~CArrow()
{
	delete m_IdleAni;
	for (int i = 0; i < (int)ArrowEffLayer::END; ++i)
	{
		delete m_DeadAni[i];
	}
	for (int i = 0; i < (int)ArrowEffLayer::END; ++i)
	{
		delete m_DeadAniG[i];
	}
}

void CArrow::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
	CMonster* pMon = dynamic_cast<CMonster*>(_Other->GetOwner());
	CLargo* pLargo = dynamic_cast<CLargo*>(_Other->GetOwner());
	if (pMon)
	{
		pMon->GiveDamage(m_ATK);
		m_PierceCount -= 1;
		if (m_PierceCount < 1)
		{
			GetRigidBody()->SetSpeed(Vec2D(0, 0));
			GetRigidBody()->SetZeroGravity(true);
			GetRigidBody()->SetAirborneSpeed(0);
			m_bFin = true;
		}
	}
	else if (pLargo)
	{
		pLargo->GiveDamage(m_ATK);
		m_PierceCount -= 1;
		if (m_PierceCount < 1)
		{
			GetRigidBody()->SetSpeed(Vec2D(0, 0));
			GetRigidBody()->SetZeroGravity(true);
			GetRigidBody()->SetAirborneSpeed(0);
			m_bFin = true;
		}
	}
}

void CArrow::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CArrow::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CArrow::Begin()
{
}

void CArrow::Tick()
{
	if (m_IdleAni)
		m_IdleAni->FinalTick();
	for (int i = 0; i < (int)ArrowEffLayer::END; ++i)
	{
		if (m_DeadAni[i])
			m_DeadAni[i]->FinalTick();
	}
	for (int i = 0; i < (int)ArrowEffLayer::END; ++i)
	{
		if (m_DeadAniG[i])
			m_DeadAniG[i]->FinalTick();
	}


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

	if (GetRigidBody()->GetAirborne() == false)
		GetRigidBody()->SetSpeed(Vec2D(0, 0));

	// 그라운드 포지션 디버그 렌더링
	CDbgRender::GetInst()->AddDbgRender(DbgRenderShape::Circle, GetGroundPos() - Vec2D(5, 5)
		, Vec2D(10, 10), 0, Color(255, 0, 0, 255));

	
	m_Lifetime -= CTimeMgr::GetInst()->GetDeltaTime();
	if (m_Lifetime < 0)
	{
		if (!m_bFuncCalled && m_DeadFunc != nullptr)
		{
			(m_DeadFuncObj->*m_DeadFunc)((DWORD_PTR)this);
			m_bFuncCalled = true;
		}
		CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(GetLayerType(), GetID());
	}
}

void CArrow::Render()
{
	if (m_bFin)
	{
		for (int i = 0; i < (int)ArrowEffLayer::END; ++i)
		{
			if (m_DeadAni[i])
				m_DeadAni[i]->Render(this, false, m_bLookLeft);
		}
		if (!m_bSoundPlayed && m_DeadSound != nullptr)
		{
			m_DeadSound->Play();
			m_bSoundPlayed = true;
		}
		if (!m_bFuncCalled && m_DeadFunc != nullptr)
		{
			(m_DeadFuncObj->*m_DeadFunc)((DWORD_PTR)this);
			m_bFuncCalled = true;
		}
	}
	else
	{
		if (GetRigidBody()->GetAirborne() == false)
		{
			for (int i = 0; i < (int)ArrowEffLayer::END; ++i)
			{
				if (m_DeadAniG[i])
					m_DeadAniG[i]->Render(this, false, m_bLookLeft);
			}
			if (!m_bSoundPlayed && m_DeadSound != nullptr)
			{
				m_DeadSound->Play();
				m_bSoundPlayed = true;
			}
			if (!m_bFuncCalled && m_DeadFunc != nullptr)
			{
				(m_DeadFuncObj->*m_DeadFunc)((DWORD_PTR)this);
				m_bFuncCalled = true;
			}
		}
		else
		{
			if (m_IdleAni)
			{
				float angle = acos(GetRigidBody()->GetSpeed().x
					/ Vec2D(GetRigidBody()->GetSpeed().x, GetRigidBody()->GetAirborneSpeed() + GetRigidBody()->GetSpeed().y).Length()) * 180.f / PI;
				m_IdleAni->SetAngle(angle * (GetRigidBody()->GetAirborneSpeed() < 0 ? -1 : 1));
				m_IdleAni->Render(this, false, m_bLookLeft);
			}
		}
	}

	if ((m_bFin && m_DeadAni[0] != nullptr && m_DeadAni[0]->GetCurSceneNum() == m_DeadAni[0]->GetFinal())
		|| (m_DeadAniG[0] != nullptr && m_DeadAniG[0]->GetCurSceneNum() == m_DeadAniG[0]->GetFinal()))
	{
		CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(GetLayerType(), GetID());
	}
}
