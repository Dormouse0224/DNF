#include "pch.h"
#include "CStickerMgr.h"
#include "CLevelMgr.h"
#include "CSticker.h"
#include "CObj.h"
#include "CKeyMgr.h"
#include "CCameraMgr.h"
#include "CUI.h"

CStickerMgr::CStickerMgr()
	: m_PrevCursorPos(0, 0)
	, m_Check(nullptr)
{}

CStickerMgr::~CStickerMgr()
{}


void CStickerMgr::Tick()
{
	// ��ƼĿ ��ü�� ���� ��� ���� ���� �������Ǵ� ������Ʈ�� ����
	m_StickerVec = CLevelMgr::GetInst()->GetCurrentLevel()->GetStickerVec();
	CSticker* pCheck = nullptr;
	for (int i = 0; i < m_StickerVec.size(); ++i)
	{
		// ���콺 ��Ŭ������ ��ư�� ���ȴ��� üũ
		Key_state MLBtn = CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN);
		if (MLBtn == Key_state::NONE)
		{
			m_StickerVec[i]->m_IsPressed = false;
		}
		else
		{
			if (MLBtn == Key_state::TAP && m_StickerVec[i]->IsCursorOn())
			{
				if (pCheck == nullptr || ObjectSort(pCheck->GetOwner(), m_StickerVec[i]->GetOwner()))
					pCheck = m_StickerVec[i];
			}
			else if (MLBtn == Key_state::RELEASE && m_StickerVec[i]->m_IsPressed)
			{
				m_StickerVec[i]->m_IsPressed = false;
				if (m_StickerVec[i]->IsCursorOn())
				{
					// ��ƼĿ Ŭ���� - ��ƼĿ�� ���� ������Ʈ�� ���õ� ������Ʈ�� ����
					if (CLevelMgr::GetInst()->GetCurrentLevel()->GetSelectedObj() != m_StickerVec[i]->GetOwner())
						CLevelMgr::GetInst()->GetCurrentLevel()->SetSelectedObj(m_StickerVec[i]->GetOwner());
				}
			}
		}
	}
	if (pCheck)
		pCheck->m_IsPressed = true;
	
	CObj* SelectedObj = CLevelMgr::GetInst()->GetCurrentLevel()->GetSelectedObj();
	Vec2D CursorPos = CKeyMgr::GetInst()->GetMousePos();
	// �巡��-���
	if (SelectedObj != nullptr && CheckObjCursorOn(SelectedObj))
	{
		Key_state MLBtn = CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN);
		if (MLBtn == Key_state::TAP)
		{
			m_PrevCursorPos = CursorPos;
		}
		else if (MLBtn == Key_state::PRESSED)
		{
			CUI* pUI = dynamic_cast<CUI*>(SelectedObj);
			if (pUI)
			{
				pUI->SetUIPos(pUI->GetUIPos() + (CursorPos - m_PrevCursorPos));
			}
			else
			{
				SelectedObj->SetLocation(SelectedObj->GetLocation() + (CursorPos - m_PrevCursorPos));
			}
			m_PrevCursorPos = CursorPos;
		}
	}

	
}

bool CStickerMgr::CheckObjCursorOn(CObj* _object)
{
	Vec2D CamPos = CCameraMgr::GetInst()->GetCameraPos();
	Vec2D CursorPos = CKeyMgr::GetInst()->GetMousePos();
	return (_object->GetLocation() - CamPos << CursorPos && _object->GetLocation() + _object->GetScale() - CamPos >> CursorPos);
}


