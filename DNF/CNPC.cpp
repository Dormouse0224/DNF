#include "pch.h"
#include "CNPC.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "CPlayer.h"
#include "CLevelMgr.h"
#include "CUI.h"
#include "CAlbumPlayer.h"

CNPC::CNPC(wstring _name)
	: CObj(_name)
	, m_reactCallback(nullptr)
	, m_NPCUI(nullptr)
	, m_IdlePlayer(nullptr)
	, m_CallbackIndex(0)
{
	SetLayerType(LayerType::Object);
}

CNPC::~CNPC()
{
}

void CNPC::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
	if (m_IdlePlayer)
		m_IdlePlayer->SetCurrentScene(0);
}

void CNPC::Overlap(CCollider* _Self, CCollider* _Other)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_Other->GetOwner());
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::X) == Key_state::TAP && pPlayer != nullptr)
	{
		// 상호작용
		if (m_reactCallback)
			m_reactCallback(this);
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
	if (m_NPCUI != nullptr && CKeyMgr::GetInst()->GetKeyState(Keyboard::ESC) == Key_state::TAP)
	{
		CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(LayerType::UI, m_NPCUI->GetID());
		m_NPCUI = nullptr;
	}
}

void CNPC::Render()
{
	CObj::Render();
}

void CNPC::InitCallback()
{
	if (m_CallbackIndex == 1)
	{

	}
}
