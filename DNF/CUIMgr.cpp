#include "pch.h"
#include "CUIMgr.h"
#include "CLevelMgr.h"
#include "CUI.h"
#include "CLevel.h"
#include "CObj.h"
#include "CKeyMgr.h"

CUIMgr::CUIMgr()
{

}


CUIMgr::~CUIMgr()
{

}

void CUIMgr::Tick()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	vector<CObj*> UIVec = pLevel->GetObjLayer(LayerType::UI);

	for (int i = 0; i < UIVec.size(); ++i)
	{
		CUI* pUI = dynamic_cast<CUI*>(UIVec[i]);
		assert(pUI);

		// 마우스 좌클릭으로 버튼이 눌렸는지 체크
		Key_state MLBtn = CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN);
		if (MLBtn == Key_state::NONE)
		{
			pUI->m_IsPressed = false;
		}
		else
		{
			if (MLBtn == Key_state::TAP && pUI->IsCursorOn())
			{
				pUI->m_IsPressed = true;
				pUI->MouseLBtnDown();
			}
			else if (MLBtn == Key_state::RELEASE && pUI->m_IsPressed)
			{
				pUI->m_IsPressed = false;
				if (pUI->IsCursorOn())
				{
					pUI->MouseLBtnClikced();
				}
				else
				{
					pUI->MouseReleased();
				}
			}
		}
	}
}
