#include "pch.h"
#include "CPlayerAoE.h"
#include "CMonster.h"
#include "CLargo.h"
#include "CCollider.h"
#include "CTimeMgr.h"
#include "CAlbumPlayer.h"
#include "CLevelMgr.h"
#include "CDbgRender.h"

CPlayerAoE::CPlayerAoE(wstring _name, float _Radius)
	: CObj(_name)
	, m_EffAni{}
	, m_ATK(0)
	, m_bAttacked(false)
	, m_Radius(_Radius)
	, m_EffSound(nullptr)
	, m_bSoundPlayed(false)
{
}

CPlayerAoE::~CPlayerAoE()
{
	for (int i = 0; i < (int)AoEEffLayer::END; ++i)
	{
		delete m_EffAni[i];
	}
}

void CPlayerAoE::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CPlayerAoE::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CPlayerAoE::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CPlayerAoE::Begin()
{
}

void CPlayerAoE::Tick()
{
	CDbgRender::GetInst()->AddDbgRender(DbgRenderShape::Circle
		, GetLocation() - Vec2D(m_Radius, m_Radius / 3.f), Vec2D(m_Radius * 2.f, m_Radius * 2.f / 3.f), 0, Color(255, 255, 0, 0));

	for (int i = 0; i < (int)AoEEffLayer::END; ++i)
	{
		if (m_EffAni[i])
			m_EffAni[i]->FinalTick();
	}

	if (!m_bAttacked)
	{
		m_bAttacked = true;
		vector<CObj*> vec = CLevelMgr::GetInst()->GetCurrentLevel()->GetObjLayer(LayerType::Object);
		CObj* pPlayer = (CObj*)CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer();
		for (int i = 0; i < vec.size(); ++i)
		{
			if (vec[i] != pPlayer && Vec2D(vec[i]->GetLocation().x - GetLocation().x, (vec[i]->GetLocation().y - GetLocation().y) * 3.f).Length() < m_Radius)
			{
				vec[i]->GiveDamage(0);
			}
		}
	}

	CheckEnd();
}

void CPlayerAoE::Render()
{
	for (int i = 0; i < (int)AoEEffLayer::END; ++i)
	{
		if (m_EffAni[i])
			m_EffAni[i]->Render(this, false, m_bLookLeft);
	}
}

void CPlayerAoE::CheckEnd()
{
	//for (int i = 0; i < (int)AoEEffLayer::END; ++i)
	//{
	//	if (m_EffAni[i])
	//	{
	//		if (m_EffAni[i]->GetCurSceneNum() != m_EffAni[i]->GetFinal())
	//			return;
	//	}
	//}
	if (m_EffAni[0])
	{
		if (m_EffAni[0]->GetCurSceneNum() != m_EffAni[0]->GetFinal())
			return;
	}
	CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(GetLayerType(), GetID());
}
