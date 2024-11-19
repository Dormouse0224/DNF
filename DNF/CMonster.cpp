#include "pch.h"
#include "CMonster.h"
#include "CAlbumPlayer.h"
#include "CDbgRender.h"
#include "CRigidBody.h"
#include "CLevelMgr.h"

CMonster::CMonster(wstring _name)
	: CObj(_name)
	, m_Animation{}
	, m_DetectRange(0)
	, m_AttackRange(0)
	, m_AttackFrame(0, 0)
	, m_AttackCol(nullptr)
	, m_MonsterTemplate(MonsterTemplate::NONE)
	, m_Hurt(false)
{
	SetLayerType(LayerType::Object);
	SetRigidBody(new CRigidBody(L"Monster_RB"));
}

CMonster::~CMonster()
{
	CLevelMgr::GetInst()->GetCurrentLevel()->SetMonsterCount(CLevelMgr::GetInst()->GetCurrentLevel()->GetMonsterCount() - 1);
}

void CMonster::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CMonster::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CMonster::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CMonster::Begin()
{
}

void CMonster::Tick()
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

	if (GetRigidBody()->GetSpeed().x < 0)
	{
		m_bLookLeft = true;
	}
	else if (GetRigidBody()->GetSpeed().x > 0)
	{
		m_bLookLeft = false;
	}

	if (GetCurHP() <= 0)
	{
		CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(GetLayerType(), GetID());
	}

	CDbgRender::GetInst()->AddDbgRender(DbgRenderShape::Circle
		, GetGroundPos() - Vec2D(m_DetectRange, m_DetectRange), Vec2D(m_DetectRange, m_DetectRange) * 2, 0, Color(255, 0, 255, 0));
	CDbgRender::GetInst()->AddDbgRender(DbgRenderShape::Circle
		, GetGroundPos() - Vec2D(m_AttackRange, m_AttackRange), Vec2D(m_AttackRange, m_AttackRange) * 2, 0, Color(255, 0, 255, 0));
}

void CMonster::Render()
{
	int state = GetState();
	for (int i = 0; i < m_Animation[state].size(); ++i)
	{
		m_Animation[state][i]->Render(this, false, m_bLookLeft);
	}
}

void CMonster::GiveDamage(int _dmg)
{
	CObj::GiveDamage(_dmg);
	m_Hurt = true;
}

void CMonster::AddAnimation(MonsterState _state, CAlbumPlayer* _animation)
{
	m_Animation[(int)_state].push_back(_animation);
	AddComponent(_animation);
}

