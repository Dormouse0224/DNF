#include "pch.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CCollider.h"
#include "CObj.h"

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
	// 이전프레임에 등록된 충돌체들 등록 해제
	for (UINT i = 0; i < (UINT)LayerType::END; ++i)
	{
		m_hCollider[i].clear();
	}

	// 오브젝트 틱 수행
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
				if ((LayerType)i == LayerType::Object)
				{
					std::sort(m_hObj[i].begin(), m_hObj[i].end(), [](const CObj* a, const CObj* b)
						{
							if (a->GetLocation().y == b->GetLocation().y)
							{
								return a->GetLocation().x < b->GetLocation().x;
							}
							else
							{
								return a->GetLocation().y < b->GetLocation().y;
							}
						});
				}
				else
				{
					m_hObj[i][j]->Render();
				}
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

void CLevel::ClearObject()
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
}


