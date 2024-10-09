#include "pch.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CCollider.h"

CLevel::CLevel(wstring _Name)
	: CBase(_Name)
{
	CLevelMgr::GetInst()->AddLevel(_Name, this);
}

CLevel::~CLevel()
{
	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		Delete_Vector(m_hObj[i]);
	}

	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		Delete_Vector(m_hCollider[i]);
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
		}
	}
}

void CLevel::Tick()
{
	// ���������ӿ� ��ϵ� �浹ü�� ��� ����
	for (UINT i = 0; i < (UINT)LayerType::END; ++i)
	{
		m_hCollider[i].clear();
	}

	// ������Ʈ ƽ ����
	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		for (int j = 0; j < m_hObj[i].size(); ++j)
		{
			m_hObj[i][j]->Tick();
		}
	}
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

void CLevel::DeleteObject()
{
	for (int i = 0; i < (int)LayerType::END; ++i)
	{
		for (int j = 0; j < m_hObj[i].size(); ++j)
		{
			if (m_hObj[i][j]->GetDead())
			{
				delete m_hObj[i][j];
				m_hObj[i].erase(m_hObj[i].begin() + j);
			}
		}
	}
	
}


