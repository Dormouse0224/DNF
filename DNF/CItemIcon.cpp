#include "pch.h"
#include "CItemIcon.h"
#include "CKeyMgr.h"
#include "CAlbumPlayer.h"
#include "CUIMgr.h"
#include "CCameraMgr.h"
#include "CLevelMgr.h"
#include "CPlayer.h"
#include "CEngine.h"
#include "CTextureMgr.h"

CItemIcon::CItemIcon(wstring _name, int _idx)
	: CUI(_name)
	, m_PrevCursorPos(0, 0)
	, m_BeginCursorPos(0, 0)
	, m_EndCursorPos(0, 0)
	, m_InventoryIndex(_idx)
{
	SetScale(Vec2D(30, 30));
}

CItemIcon::~CItemIcon()
{
}

void CItemIcon::Tick_UI()
{
	if (m_IsPressed)
	{
		SetUIPos(GetUIPos() + (CKeyMgr::GetInst()->GetMousePos() - m_PrevCursorPos));
		m_PrevCursorPos = CKeyMgr::GetInst()->GetMousePos();
	}

	if (m_PrevPressed == true && m_IsPressed == false)
	{
		DragEnd();
	}
}

void CItemIcon::Render_UI()
{
	for (int i = 0; i < m_AlbumPlayerVector.size(); ++i)
	{
		m_AlbumPlayerVector[i]->Render(this);
	}
}

void CItemIcon::MouseLBtnDown()
{
	m_PrevCursorPos = CKeyMgr::GetInst()->GetMousePos();
	m_BeginCursorPos = CKeyMgr::GetInst()->GetMousePos();
	CUIMgr::GetInst()->SetRenderDelegate0(this, (DELEGATE_0)&CItemIcon::Render_UI);
}

void CItemIcon::MouseLBtnClikced()
{
	
}

void CItemIcon::MouseReleased()
{

}

void CItemIcon::DragEnd()
{
	m_EndCursorPos = CKeyMgr::GetInst()->GetMousePos();
	CUIMgr::GetInst()->ResetRender();

	// 드래그-드랍 이동중이었던 경우
	if ((m_EndCursorPos - m_BeginCursorPos).Length() > 5)
	{
		CUI* pDropUI = CUIMgr::GetInst()->GetPriorityUI(CUIMgr::GetInst()->GetSuperParent(this));
		CItemIcon* pDestItemUI = nullptr;
		ItemInfo* pDestItem = nullptr;
		if (pDropUI == this)
		{
			// 최상위 UI가 자신인경우 - 최상위에서 두번째 UI를 드랍UI로 지정
			pDropUI = CUIMgr::GetInst()->GetPriorityUI(CUIMgr::GetInst()->GetSuperParent(this), true);
			if (dynamic_cast<CItemIcon*>(pDropUI))
			{
				// 최상위에서 두번째 UI가 다른 아이콘인 경우 - 세번째를 드랍으로 지정 및 다른 아이콘 기억
				pDestItemUI = (CItemIcon*)pDropUI;
				pDropUI = CUIMgr::GetInst()->GetPriorityUI(CUIMgr::GetInst()->GetSuperParent(this), false, true);
			}
		}
		else if (dynamic_cast<CItemIcon*>(pDropUI))
		{
			// 최상위 UI가 다른 아이콘인경우 - 다른 아이콘의 부모를 드랍으로 지정 및 다른 아이콘 기억
			pDestItemUI = (CItemIcon*)pDropUI;
			pDropUI = pDestItemUI->GetParent();
		}

		if (pDropUI)
		{
			if (pDropUI->GetUIItemType() == m_ItemInfo->Item_type)
			{
				// 드랍된 인벤토리의 저장 아이템 타입이 일치함
				if (pDropUI->GetUIInventoryType() == InventoryType::Slot)
				{
					// 드랍된 인벤토리가 슬롯(격자형) 타입인 경우
					// 드랍된 행렬을 계산
					Vec2D DestVec = (m_EndCursorPos - pDropUI->GetLocation() + CCameraMgr::GetInst()->GetCameraPos()) / 30;
					// 플레이어 객체의 인벤토리 정보를 수정 (목적지 인벤토리 정보에 추가, 출발지 인벤토리 정보에서 삭제)
					vector<ItemInfo*>& DestInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetInvenVec(m_ItemInfo->Item_type);
					if (DestInventory[(int)DestVec.x + (int)DestVec.y * 8])
					{
						pDestItem = DestInventory[(int)DestVec.x + (int)DestVec.y * 8];
					}
					DestInventory[(int)DestVec.x + (int)DestVec.y * 8] = m_ItemInfo;
					if (m_Parent->GetUIInventoryType() == InventoryType::Slot)
					{
						// 출발지 인벤토리가 슬롯 타입인 경우
						vector<ItemInfo*>& SrcInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetInvenVec(m_ItemInfo->Item_type);
						SrcInventory[m_InventoryIndex] = pDestItem;
						if (pDestItemUI)
						{
							pDestItemUI->SetUIPos(Vec2D(m_InventoryIndex % 8, m_InventoryIndex / 8) * 30);
							pDestItemUI->m_InventoryIndex = m_InventoryIndex;
						}
						m_InventoryIndex = (int)DestVec.x + (int)DestVec.y * 8;
					}
					else if (m_Parent->GetUIInventoryType() == InventoryType::Equip && m_ItemInfo->Item_type == ItemType::Equip)
					{
						// 출발지 인벤토리가 장착장비 타입의 장비 인벤토리인 경우
						vector<ItemInfo*>& SrcInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetEquipVec();
						SrcInventory[m_InventoryIndex] = pDestItem;
						if (pDestItemUI)
						{
							pDestItemUI->SetEquipIconPos(m_InventoryIndex);
							pDestItemUI->m_InventoryIndex = m_InventoryIndex;
						}
						m_InventoryIndex = (int)DestVec.x + (int)DestVec.y * 8;
					}
					else if (m_Parent->GetUIInventoryType() == InventoryType::Equip && m_ItemInfo->Item_type == ItemType::Avatar)
					{
						// 출발지 인벤토리가 장착장비 타입의 아바타 인벤토리인 경우
						vector<ItemInfo*>& SrcInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetAvatarEquipVec();
						SrcInventory[m_InventoryIndex] = pDestItem;
						if (pDestItemUI)
						{
							pDestItemUI->SetAvatarIconPos(m_InventoryIndex);
							pDestItemUI->m_InventoryIndex = m_InventoryIndex;
						}
						m_InventoryIndex = (int)DestVec.x + (int)DestVec.y * 8;
					}
					// 아이콘 UI의 위치를 슬롯에 맞춤
					SetUIPos(Vec2D((int)DestVec.x, (int)DestVec.y) * 30);
					// 부모가 달라진경우 갱신
					if (pDropUI != m_Parent)
					{
						CUI* temp = m_Parent;
						vector<CUI*>& oldChVec = m_Parent->GetChildVec();
						oldChVec.erase(find(oldChVec.begin(), oldChVec.end(), this));
						pDropUI->GetChildVec().push_back(this);
						m_Parent = pDropUI;
						if (pDestItemUI)
						{
							vector<CUI*>& oldChVec1 = pDestItemUI->GetParent()->GetChildVec();
							oldChVec1.erase(find(oldChVec1.begin(), oldChVec1.end(), pDestItemUI));
							temp->GetChildVec().push_back(pDestItemUI);
							pDestItemUI->SetParent(temp);
						}
					}
				}
				else if (pDropUI->GetUIInventoryType() == InventoryType::Equip && m_ItemInfo->Item_type == ItemType::Equip)
				{
					// 플레이어 객체의 인벤토리 정보를 수정 (목적지 인벤토리 정보에 추가, 출발지 인벤토리 정보에서 삭제)
					vector<ItemInfo*>& DestInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetEquipVec();
					if (DestInventory[m_ItemInfo->Item_secondtype])
					{
						pDestItem = DestInventory[m_ItemInfo->Item_secondtype];
					}
					DestInventory[m_ItemInfo->Item_secondtype] = m_ItemInfo;
					if (m_Parent->GetUIInventoryType() == InventoryType::Slot)
					{
						// 출발지 인벤토리가 슬롯 타입인 경우
						vector<ItemInfo*>& SrcInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetInvenVec(m_ItemInfo->Item_type);
						SrcInventory[m_InventoryIndex] = pDestItem;
						if (pDestItemUI)
						{
							pDestItemUI->SetUIPos(Vec2D(m_InventoryIndex % 8, m_InventoryIndex / 8) * 30);
							pDestItemUI->m_InventoryIndex = m_InventoryIndex;
						}
						m_InventoryIndex = m_ItemInfo->Item_secondtype;
					}
					// 아이콘 UI의 위치를 슬롯에 맞춤
					SetEquipIconPos(m_InventoryIndex);
					// 부모가 달라진경우 갱신
					if (pDropUI != m_Parent)
					{
						CUI* temp = m_Parent;
						vector<CUI*>& oldChVec = m_Parent->GetChildVec();
						oldChVec.erase(find(oldChVec.begin(), oldChVec.end(), this));
						pDropUI->GetChildVec().push_back(this);
						m_Parent = pDropUI;
						if (pDestItemUI)
						{
							vector<CUI*>& oldChVec1 = pDestItemUI->GetParent()->GetChildVec();
							oldChVec1.erase(find(oldChVec1.begin(), oldChVec1.end(), pDestItemUI));
							temp->GetChildVec().push_back(pDestItemUI);
							pDestItemUI->SetParent(temp);
						}
					}
				}
				else if (pDropUI->GetUIInventoryType() == InventoryType::Equip && m_ItemInfo->Item_type == ItemType::Avatar)
				{
					// 플레이어 객체의 인벤토리 정보를 수정 (목적지 인벤토리 정보에 추가, 출발지 인벤토리 정보에서 삭제)
					vector<ItemInfo*>& DestInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetAvatarEquipVec();
					if (DestInventory[m_ItemInfo->Item_secondtype])
					{
						pDestItem = DestInventory[m_ItemInfo->Item_secondtype];
					}
					DestInventory[m_ItemInfo->Item_secondtype] = m_ItemInfo;
					if (m_Parent->GetUIInventoryType() == InventoryType::Slot)
					{
						// 출발지 인벤토리가 슬롯 타입인 경우
						vector<ItemInfo*>& SrcInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetInvenVec(m_ItemInfo->Item_type);
						SrcInventory[m_InventoryIndex] = pDestItem;
						if (pDestItemUI)
						{
							pDestItemUI->SetUIPos(Vec2D(m_InventoryIndex % 8, m_InventoryIndex / 8) * 30);
							pDestItemUI->m_InventoryIndex = m_InventoryIndex;
						}
						m_InventoryIndex = m_ItemInfo->Item_secondtype;
					}
					// 아이콘 UI의 위치를 슬롯에 맞춤
					SetAvatarIconPos(m_InventoryIndex);
					// 부모가 달라진경우 갱신
					if (pDropUI != m_Parent)
					{
						CUI* temp = m_Parent;
						vector<CUI*>& oldChVec = m_Parent->GetChildVec();
						oldChVec.erase(find(oldChVec.begin(), oldChVec.end(), this));
						pDropUI->GetChildVec().push_back(this);
						m_Parent = pDropUI;
						if (pDestItemUI)
						{
							vector<CUI*>& oldChVec1 = pDestItemUI->GetParent()->GetChildVec();
							oldChVec1.erase(find(oldChVec1.begin(), oldChVec1.end(), pDestItemUI));
							temp->GetChildVec().push_back(pDestItemUI);
							pDestItemUI->SetParent(temp);
						}
					}
				}
			}
			else
			{
				SetUIPos(GetUIPos() - (m_EndCursorPos - m_BeginCursorPos));
			}
		}
		else
		{
			SetUIPos(GetUIPos() - (m_EndCursorPos - m_BeginCursorPos));
		}
	}
	else
	{
		// 단순클릭
		SetUIPos(GetUIPos() - (m_EndCursorPos - m_BeginCursorPos));
	}
}

void CItemIcon::SetItemInfo(ItemInfo* _info)
{
	m_ItemInfo = _info;
	AddComponent(CAlbumPlayer::CreatePlayerFromFile(_info->Texture_ani
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + _info->Texture_ani + L".animation"));
}

void CItemIcon::SetEquipIconPos(int _idx)
{
	switch ((EquipParts)_idx)
	{
	case EquipParts::Weapon:
		SetUIPos(Vec2D(175, 0));
		break;
	case EquipParts::Top:
		SetUIPos(Vec2D(0, 0));
		break;
	case EquipParts::Bottom:
		SetUIPos(Vec2D(0, 33));
		break;
	case EquipParts::Shoulder:
		SetUIPos(Vec2D(33, 0));
		break;
	case EquipParts::Belt:
		SetUIPos(Vec2D(33, 33));
		break;
	case EquipParts::Shoes:
		SetUIPos(Vec2D(0, 66));
		break;
	case EquipParts::Neckless:
		SetUIPos(Vec2D(208, 33));
		break;
	case EquipParts::Amulet:
		SetUIPos(Vec2D(175, 33));
		break;
	case EquipParts::Ring:
		SetUIPos(Vec2D(208, 66));
		break;
	}
}

void CItemIcon::SetAvatarIconPos(int _idx)
{
	switch ((AvatarParts)_idx)
	{
	case AvatarParts::Hair:
		SetUIPos(Vec2D(140, 10));
		break;
	case AvatarParts::Cap:
		SetUIPos(Vec2D(173, 10));
		break;
	case AvatarParts::Pants:
		SetUIPos(Vec2D(173, 76));
		break;
	case AvatarParts::Coat:
		SetUIPos(Vec2D(173, 43));
		break;
	case AvatarParts::Shoes:
		SetUIPos(Vec2D(206, 76));
		break;
	case AvatarParts::LBow:
		SetUIPos(Vec2D(107, 10));
		break;
	}
}