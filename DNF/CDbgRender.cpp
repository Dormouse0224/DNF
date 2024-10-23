#include "pch.h"
#include "CDbgRender.h"
#include "CEngine.h"
#include "CTimeMgr.h"
#include "CTextureMgr.h"

CDbgRender::CDbgRender()
	: m_Show(false)
	, m_DbgRenderList{}
{
}

CDbgRender::~CDbgRender()
{
}

void CDbgRender::AddDbgRender(DbgRenderShape _Shape, Vec2D _Coord0, Vec2D _Coord1, float _Duration, Color _color)
{
	if (!m_Show)
		return;

	m_DbgRenderList.push_back(DbgRenderInfo(_Shape, _Coord0, _Coord1, _Duration, _color));
}

void CDbgRender::Render()
{
	//HDC dc = CEngine::GetInst()->GetSubDC();

	for (list<DbgRenderInfo>::iterator iter = m_DbgRenderList.begin(); iter != m_DbgRenderList.end(); ++iter)
	{
		switch (iter->m_Shape)
		{
		case DbgRenderShape::Rectangle:
			CTextureMgr::GetInst()->DrawRect(iter->m_Color, 2, iter->m_Coord0, iter->m_Coord1, false);
			//Rectangle(dc, (int)iter->m_Coord0.x, (int)iter->m_Coord0.y, (int)iter->m_Coord1.x, (int)iter->m_Coord1.y);
			break;
		case DbgRenderShape::Circle:
			//Ellipse(dc, (int)iter->m_Coord0.x, (int)iter->m_Coord0.y, (int)iter->m_Coord1.x, (int)iter->m_Coord1.y);
			break;
		case DbgRenderShape::Line:
			CTextureMgr::GetInst()->DrawLine(iter->m_Color, 2, iter->m_Coord0, iter->m_Coord1, false);
			//MoveToEx(dc, (int)iter->m_Coord0.x, (int)iter->m_Coord0.y, nullptr);
			//LineTo(dc, (int)iter->m_Coord1.x, (int)iter->m_Coord1.y);
			break;
		}

		iter->m_Time += DT;
		
		if (iter->m_Duration <= iter->m_Time)
		{
			iter = m_DbgRenderList.erase(iter);
		}
		else
		{
			++iter;
		}
	}


}
