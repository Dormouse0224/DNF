#include "pch.h"
#include "CPortal.h"
#include "CLevelMgr.h"
#include "CStage.h"
#include "CAlbumPlayer.h"
#include "CDbgRender.h"
#include "CPlayer.h"
#include "CCollider.h"

CPortal::CPortal(wstring _name)
	: CObj(_name)
	, m_bActive(true)
	, m_Dest(nullptr)
	, m_DeactiveAnimation{}
{
	SetLayerType(LayerType::Object);
}

CPortal::~CPortal()
{
	for (int i = 0; i < m_DeactiveAnimation.size(); ++i)
	{
		if (m_DeactiveAnimation[i])
		{
			delete m_DeactiveAnimation[i];
		}
	}
	m_DeactiveAnimation.clear();
}

void CPortal::Overlap(CCollider* _Self, CCollider* _Other)
{
	if (m_bActive && m_Dest != nullptr && (_Self->GetOwner()->GetGroundPos() - _Other->GetOwner()->GetGroundPos()).Length() < m_DetectRange)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(_Other->GetOwner());
		if (pPlayer)
			CLevelMgr::GetInst()->ChangeLevel(m_Dest);
	}
}



void CPortal::Begin()
{

}

void CPortal::Tick()
{

}

void CPortal::Render()
{
	if (m_bActive)
	{
		CObj::Render();
	}
	else
	{
		for (int i = 0; i < m_DeactiveAnimation.size(); ++i)
		{
			m_DeactiveAnimation[i]->Render(this);
		}
	}
}

