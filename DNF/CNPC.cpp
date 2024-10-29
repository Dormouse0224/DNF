#include "pch.h"
#include "CNPC.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "CPlayer.h"

CNPC::CNPC(wstring _name)
	: CObj(_name)
	, m_reactCallback(nullptr)
{
	SetLayerType(LayerType::Object);
}

CNPC::~CNPC()
{
}

void CNPC::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CNPC::Overlap(CCollider* _Self, CCollider* _Other)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_Other->GetOwner());
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::X) == Key_state::TAP && pPlayer != nullptr)
	{
		// 상호작용
		if (m_reactCallback)
			m_reactCallback();
	}
}

void CNPC::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CNPC::Begin()
{
}

void CNPC::Tick()
{
}

void CNPC::Render()
{
	CObj::Render();
}
