#include "pch.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CCollider.h"
#include "CObj.h"
#include "CTaskMgr.h"
#include "CDbgRender.h"
#include "CSticker.h"
#include "CPortal.h"

CLevel::CLevel(wstring _Name)
	: CBase(_Name)
	, m_SelectedObj(nullptr)
	, m_Player(nullptr)
	, m_CameraFollowPlayer(false)
	, m_MonsterCount(0)
{
	CLevelMgr::GetInst()->AddLevel(_Name, this);
}

CLevel::~CLevel()
{
	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		Delete_Vector(m_hObj[i]);
	}
}



void CLevel::Begin()
{
	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		if (m_hObj[i].empty())
			continue;
		for (int j = 0; j < m_hObj[i].size(); ++j)
		{
			m_hObj[i][j]->Begin();
			if (m_hObj[i][j]->GetName() == L"Player")
				m_Player = (CPlayer*)m_hObj[i][j];
		}
	}
}

void CLevel::Tick()
{
	// 이전프레임에 등록된 충돌체들 등록 해제
	for (UINT i = 0; i < (UINT)LayerType::END; ++i)
	{
		m_hCollider[i].clear();
	}

	// 이전프레임에 등록된 스티커 등록 해제
	m_hSticker.clear();

	// 오브젝트 틱 수행
	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		for (int j = 0; j < m_hObj[i].size(); ++j)
		{
			if (m_hObj[i][j]->GetDead() == false)
			{
				m_hObj[i][j]->Tick();
			}
		}
	}

	// 맵오브젝트 정렬
	std::sort(m_hObj[(int)LayerType::Object].begin(), m_hObj[(int)LayerType::Object].end(), ObjectSort);
}

void CLevel::FinalTick()
{
	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		for (int j = 0; j < m_hObj[i].size(); ++j)
		{
			m_hObj[i][j]->FinalTick();
		}
	}
}


void CLevel::Render()
{
	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		for (int j = 0; j < m_hObj[i].size(); ++j)
		{
			if (m_hObj[i][j]->GetDead() == false)
			{
				m_hObj[i][j]->Render();
			}
		}
	}
}

void CLevel::DeleteObject(LayerType _layer, UINT _ObjectID)
{
	for (int i = 0; i < m_hObj[(int)_layer].size(); ++i)
	{
		if (m_hObj[(int)_layer][i]->GetID() == _ObjectID)
		{
			StTask deleteobj;
			deleteobj.m_TaskType = TaskType::ObjectDelete;
			deleteobj.m_param0 = (DWORD_PTR)m_hObj[(int)_layer][i];
			CTaskMgr::GetInst()->AddTask(deleteobj);
			m_hObj[(int)_layer].erase(m_hObj[(int)_layer].begin() + i);
		}
	}
	
}

void CLevel::EraseCollider(int _ID, LayerType _Layer)
{
	for (vector<CCollider*>::iterator iter = m_hCollider[(int)_Layer].begin(); iter != m_hCollider[(int)_Layer].end();)
	{
		if ((*iter)->GetID() == _ID)
		{
			iter = m_hCollider[(int)_Layer].erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CLevel::EraseSticker(int _ID)
{
	for (vector<CSticker*>::iterator iter = m_hSticker.begin(); iter != m_hSticker.end();)
	{
		if ((*iter)->GetID() == _ID)
		{
			iter = m_hSticker.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}




void CLevel::DeleteSelectedObj()
{
	if (m_SelectedObj)
	{
		for (vector<CObj*>::iterator iter = m_hObj[(int)LayerType::Object].begin(); iter != m_hObj[(int)LayerType::Object].end(); ++iter)
		{
			if (m_SelectedObj->GetID() == (*iter)->GetID() && dynamic_cast<CPortal*>(*iter) == nullptr)
			{
				delete (*iter);
				m_hObj[(int)LayerType::Object].erase(iter);
				m_SelectedObj = nullptr;
				break;
			}
		}
	}
}

void CLevel::ClearAll()
{
	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		for (int j = 0; j < m_hObj[i].size(); ++j)
		{
			if (m_hObj[i][j] != nullptr)
			{
				delete m_hObj[i][j];
				m_hObj[i][j] = nullptr;
			}
		}
		m_hObj[i].clear();
	}
	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		m_hCollider[i].clear();
	}
	m_hSticker.clear();
}


