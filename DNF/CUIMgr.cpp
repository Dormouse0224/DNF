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
	// 현재 레벨에서 UI 오브젝트를 가져옴
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	vector<CObj*> UIVec = pLevel->GetObjLayer(LayerType::UI);

	for (vector<CObj*>::reverse_iterator riter = UIVec.rbegin(); riter != UIVec.rend(); ++riter)
	{
		CUI* pParentUI = dynamic_cast<CUI*>(*riter);
		assert(pParentUI);

		// 최상위 부모 UI 포함, 본인의 자식들 중 가장 우선순위가 높은 UI 를 찾아서 반환한다.
		CUI* pUI = GetPriorityUI(pParentUI);
		if (nullptr == pUI)
		{
			ReleaseCheck(pParentUI);
			continue;
		}

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

CUI* CUIMgr::GetPriorityUI(CUI* _ParentUI)
{
	CUI* pPriorityUI = nullptr;

	// Queue 역할을 할 List
	static list<CUI*> queue;
	queue.clear();

	// 최상위 부모 UI 입력
	queue.push_back(_ParentUI);

	// Queue 가 비어있지 않다면 반복문 실행
	while (!queue.empty())
	{
		CUI* pUI = queue.front();
		queue.pop_front();

		for (size_t i = 0; i < pUI->m_ChildVec.size(); ++i)
		{
			queue.push_back(pUI->m_ChildVec[i]);
		}

		if (pUI->IsCursorOn())
			pPriorityUI = pUI;
	}

	return pPriorityUI;
}

void CUIMgr::ReleaseCheck(CUI* _UI)
{
	// Queue 역할을 할 List
	static list<CUI*> queue;
	queue.clear();

	// 최상위 부모 UI 입력
	queue.push_back(_UI);

	// Queue 가 비어있지 않다면 반복문 실행
	while (!queue.empty())
	{
		CUI* pUI = queue.front();
		queue.pop_front();

		for (size_t i = 0; i < pUI->m_ChildVec.size(); ++i)
		{
			queue.push_back(pUI->m_ChildVec[i]);
		}

		if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::RELEASE)
			pUI->m_IsPressed = false;
	}
}