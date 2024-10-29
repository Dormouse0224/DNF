#include "pch.h"
#include "CMonster.h"
#include "CAlbumPlayer.h"
#include "CDbgRender.h"

CMonster::CMonster(wstring _name)
	: CObj(_name)
	, m_Animation{}
	, m_DetectRange(0)
	, m_AttackRange(0)
	, m_AttackFrame(0, 0)
	, m_AttackCol(nullptr)
	, m_MonsterTemplate(MonsterTemplate::NONE)
{
	SetLayerType(LayerType::Object);
}

CMonster::~CMonster()
{

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
		m_Animation[state][i]->Render(this);
	}
}

void CMonster::AddAnimation(MonsterState _state, CAlbumPlayer* _animation)
{
	m_Animation[(int)_state].push_back(_animation);
	AddComponent(_animation);
}

