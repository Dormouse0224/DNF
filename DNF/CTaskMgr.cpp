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
	// garbage 벡터에 저장된 삭제 예정 상태의 오브젝트 모두 제거
	for (int i = 0; i < m_Garbage.size(); ++i)
	{
		if (m_Garbage[i] != nullptr)
			delete m_Garbage[i];
	}
	m_Garbage.clear();


	// 태스크 매니저에 요청된 태스크 처리
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
			// 오브젝트를 삭제 예정 상태로 전환 및 garbage 벡터에 저장
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
	// 요청된 태스크 모두 제거
	m_TaskQueue.clear();
}

