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
	// ���� �������� UI ������Ʈ�� ������
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	vector<CObj*> UIVec = pLevel->GetObjLayer(LayerType::UI);

	for (vector<CObj*>::reverse_iterator riter = UIVec.rbegin(); riter != UIVec.rend(); ++riter)
	{
		CUI* pParentUI = dynamic_cast<CUI*>(*riter);
		assert(pParentUI);

		// �ֻ��� �θ� UI ����, ������ �ڽĵ� �� ���� �켱������ ���� UI �� ã�Ƽ� ��ȯ�Ѵ�.
		CUI* pUI = GetPriorityUI(pParentUI);
		if (nullptr == pUI)
		{
			ReleaseCheck(pParentUI);
			continue;
		}

		// ���콺 ��Ŭ������ ��ư�� ���ȴ��� üũ
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

	// Queue ������ �� List
	static list<CUI*> queue;
	queue.clear();

	// �ֻ��� �θ� UI �Է�
	queue.push_back(_ParentUI);

	// Queue �� ������� �ʴٸ� �ݺ��� ����
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
	// Queue ������ �� List
	static list<CUI*> queue;
	queue.clear();

	// �ֻ��� �θ� UI �Է�
	queue.push_back(_UI);

	// Queue �� ������� �ʴٸ� �ݺ��� ����
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