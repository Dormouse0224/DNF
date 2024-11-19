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
			pUI->m_PrevPressed = pUI->m_IsPressed;
			pUI->m_IsPressed = false;
		}
		else
		{
			if (MLBtn == Key_state::TAP && pUI->IsCursorOn())
			{
				pUI->m_PrevPressed = pUI->m_IsPressed;
				pUI->m_IsPressed = true;
				pUI->MouseLBtnDown();

				// vector ���� ���� �� ���� �ڷ� �����ش�.
				UIVec.erase((++riter).base());
				UIVec.push_back(pParentUI);

				break;
			}
			else if (MLBtn == Key_state::RELEASE && pUI->m_IsPressed)
			{
				pUI->m_PrevPressed = pUI->m_IsPressed;
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

CUI* CUIMgr::GetPriorityUI(CUI* _ParentUI, bool _getSecondary, bool _getThird)
{
	CUI* pPriorityUI = nullptr;
	CUI* pSecondPriorityUI = nullptr;
	CUI* pThirdUI = nullptr;

	// Queue ������ �� List
	static list<CUI*> queue;
	queue.clear();

	// �ֻ��� �θ� UI �Է�
	queue.push_back(_ParentUI);

	// Queue �� ������� �ʴٸ� �ݺ��� ����
	while (!queue.empty())
	{
		CUI* pUI = queue.back();
		queue.pop_back();

		for (size_t i = pUI->m_ChildVec.size(); i > 0; --i)
		{
			queue.push_back(pUI->m_ChildVec[i - 1]);
		}

		if (pUI->IsCursorOn())
		{
			if (pSecondPriorityUI)
				pThirdUI = pSecondPriorityUI;
			if (pPriorityUI)
				pSecondPriorityUI = pPriorityUI;
			pPriorityUI = pUI;
		}
	}

	if (_getThird)
		return pThirdUI;
	if (_getSecondary)
		return pSecondPriorityUI;
	else
		return pPriorityUI;
}

CUI* CUIMgr::GetSuperParent(CUI* _UI)
{
	CUI* pSuperParentUI = _UI;
	while (pSuperParentUI->m_Parent)
	{
		pSuperParentUI = pSuperParentUI->m_Parent;
	}
	return pSuperParentUI;
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

void CUIMgr::Render()
{
	if (m_RenderDelegate0)
		(m_DelegateObj->*m_RenderDelegate0)();
}
