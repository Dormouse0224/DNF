#include "pch.h"
#include "CTaskMgr.h"
#include "CLevelMgr.h"
#include "CObj.h"

CTaskMgr::CTaskMgr()
	: m_TaskQueue{}
{

}

CTaskMgr::~CTaskMgr()
{

}

void CTaskMgr::Tick()
{
	// garbage ���Ϳ� ����� ���� ���� ������ ������Ʈ ��� ����
	for (int i = 0; i < m_Garbage.size(); ++i)
	{
		if (m_Garbage[i] != nullptr)
			delete m_Garbage[i];
	}
	m_Garbage.clear();


	// �½�ũ �Ŵ����� ��û�� �½�ũ ó��
	for (int i = 0; i < m_TaskQueue.size(); ++i)
	{
		switch (m_TaskQueue[i].m_TaskType)
		{
		case TaskType::ObjectCreate:
		{
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject((CObj*)m_TaskQueue[i].m_param0, (LayerType)m_TaskQueue[i].m_param1);
			break;
		}			
		case TaskType::ObjectDelete:
		{
			// ������Ʈ�� ���� ���� ���·� ��ȯ �� garbage ���Ϳ� ����
			CObj* pObject = (CObj*)m_TaskQueue[i].m_param0;
			if (pObject->GetDead() == false)
			{
				pObject->SetDead(true);
				m_Garbage.push_back(pObject);
			}
			break;
		}			
		case TaskType::ChangeLevel:
		{
			CLevelMgr::GetInst()->GetCurrentLevel()->End();
			CLevelMgr::GetInst()->SetCurrentLevel((CLevel*)m_TaskQueue[i].m_param0);
			CLevelMgr::GetInst()->GetCurrentLevel()->Begin();
		}
			break;
		}
	}
	// ��û�� �½�ũ ��� ����
	m_TaskQueue.clear();
}

