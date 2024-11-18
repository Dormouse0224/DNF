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

	// �巡��-��� �̵����̾��� ���
	if ((m_EndCursorPos - m_BeginCursorPos).Length() > 5)
	{
		CUI* pDropUI = CUIMgr::GetInst()->GetPriorityUI(CUIMgr::GetInst()->GetSuperParent(this));
		CItemIcon* pDestItemUI = nullptr;
		ItemInfo* pDestItem = nullptr;
		if (pDropUI == this)
		{
			// �ֻ��� UI�� �ڽ��ΰ�� - �ֻ������� �ι�° UI�� ���UI�� ����
			pDropUI = CUIMgr::GetInst()->GetPriorityUI(CUIMgr::GetInst()->GetSuperParent(this), true);
			if (dynamic_cast<CItemIcon*>(pDropUI))
			{
				// �ֻ������� �ι�° UI�� �ٸ� �������� ��� - ����°�� ������� ���� �� �ٸ� ������ ���
				pDestItemUI = (CItemIcon*)pDropUI;
				pDropUI = CUIMgr::GetInst()->GetPriorityUI(CUIMgr::GetInst()->GetSuperParent(this), false, true);
			}
		}
		else if (dynamic_cast<CItemIcon*>(pDropUI))
		{
			// �ֻ��� UI�� �ٸ� �������ΰ�� - �ٸ� �������� �θ� ������� ���� �� �ٸ� ������ ���
			pDestItemUI = (CItemIcon*)pDropUI;
			pDropUI = pDestItemUI->GetParent();
		}

		if (pDropUI)
		{
			if (pDropUI->GetUIItemType() == m_ItemInfo->Item_type)
			{
				// ����� �κ��丮�� ���� ������ Ÿ���� ��ġ��
				if (pDropUI->GetUIInventoryType() == InventoryType::Slot)
				{
					// ����� �κ��丮�� ����(������) Ÿ���� ���
					// ����� ����� ���
					Vec2D DestVec = (m_EndCursorPos - pDropUI->GetLocation() + CCameraMgr::GetInst()->GetCameraPos()) / 30;
					// �÷��̾� ��ü�� �κ��丮 ������ ���� (������ �κ��丮 ������ �߰�, ����� �κ��丮 �������� ����)
					vector<ItemInfo*>& DestInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetInvenVec(m_ItemInfo->Item_type);
					if (DestInventory[(int)DestVec.x + (int)DestVec.y * 8])
					{
						pDestItem = DestInventory[(int)DestVec.x + (int)DestVec.y * 8];
					}
					DestInventory[(int)DestVec.x + (int)DestVec.y * 8] = m_ItemInfo;
					if (m_Parent->GetUIInventoryType() == InventoryType::Slot)
					{
						// ����� �κ��丮�� ���� Ÿ���� ���
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
						// ����� �κ��丮�� ������� Ÿ���� ��� �κ��丮�� ���
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
						// ����� �κ��丮�� ������� Ÿ���� �ƹ�Ÿ �κ��丮�� ���
						vector<ItemInfo*>& SrcInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetAvatarEquipVec();
						SrcInventory[m_InventoryIndex] = pDestItem;
						if (pDestItemUI)
						{
							pDestItemUI->SetAvatarIconPos(m_InventoryIndex);
							pDestItemUI->m_InventoryIndex = m_InventoryIndex;
						}
						m_InventoryIndex = (int)DestVec.x + (int)DestVec.y * 8;
					}
					// ������ UI�� ��ġ�� ���Կ� ����
					SetUIPos(Vec2D((int)DestVec.x, (int)DestVec.y) * 30);
					// �θ� �޶������ ����
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
					// �÷��̾� ��ü�� �κ��丮 ������ ���� (������ �κ��丮 ������ �߰�, ����� �κ��丮 �������� ����)
					vector<ItemInfo*>& DestInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetEquipVec();
					if (DestInventory[m_ItemInfo->Item_secondtype])
					{
						pDestItem = DestInventory[m_ItemInfo->Item_secondtype];
					}
					DestInventory[m_ItemInfo->Item_secondtype] = m_ItemInfo;
					if (m_Parent->GetUIInventoryType() == InventoryType::Slot)
					{
						// ����� �κ��丮�� ���� Ÿ���� ���
						vector<ItemInfo*>& SrcInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetInvenVec(m_ItemInfo->Item_type);
						SrcInventory[m_InventoryIndex] = pDestItem;
						if (pDestItemUI)
						{
							pDestItemUI->SetUIPos(Vec2D(m_InventoryIndex % 8, m_InventoryIndex / 8) * 30);
							pDestItemUI->m_InventoryIndex = m_InventoryIndex;
						}
						m_InventoryIndex = m_ItemInfo->Item_secondtype;
					}
					// ������ UI�� ��ġ�� ���Կ� ����
					SetEquipIconPos(m_InventoryIndex);
					// �θ� �޶������ ����
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
					// �÷��̾� ��ü�� �κ��丮 ������ ���� (������ �κ��丮 ������ �߰�, ����� �κ��丮 �������� ����)
					vector<ItemInfo*>& DestInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetAvatarEquipVec();
					if (DestInventory[m_ItemInfo->Item_secondtype])
					{
						pDestItem = DestInventory[m_ItemInfo->Item_secondtype];
					}
					DestInventory[m_ItemInfo->Item_secondtype] = m_ItemInfo;
					if (m_Parent->GetUIInventoryType() == InventoryType::Slot)
					{
						// ����� �κ��丮�� ���� Ÿ���� ���
						vector<ItemInfo*>& SrcInventory = CLevelMgr::GetInst()->GetCurrentLevel()->GetPlayer()->GetInvenVec(m_ItemInfo->Item_type);
						SrcInventory[m_InventoryIndex] = pDestItem;
						if (pDestItemUI)
						{
							pDestItemUI->SetUIPos(Vec2D(m_InventoryIndex % 8, m_InventoryIndex / 8) * 30);
							pDestItemUI->m_InventoryIndex = m_InventoryIndex;
						}
						m_InventoryIndex = m_ItemInfo->Item_secondtype;
					}
					// ������ UI�� ��ġ�� ���Կ� ����
					SetAvatarIconPos(m_InventoryIndex);
					// �θ� �޶������ ����
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
		// �ܼ�Ŭ��
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