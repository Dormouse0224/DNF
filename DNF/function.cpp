#include "pch.h"
#include "function.h"
#include "CObj.h"
#include "CTaskMgr.h"

void ObjectCreate(CObj* _obj, LayerType _layer)
{
	StTask task = { TaskType::ObjectCreate, (DWORD_PTR)_obj, (DWORD_PTR)_layer };
	CTaskMgr::GetInst()->AddTask(task);
}

void ObjectDelete(CObj* _obj, LayerType _layer)
{
	StTask task = { TaskType::ObjectDelete, (DWORD_PTR)_obj, (DWORD_PTR)_layer };
	CTaskMgr::GetInst()->AddTask(task);
}

// a가 b 보다 뒤에(멀리) 있는가?
bool ObjectSort(const CObj* a, const CObj* b)
{
	if (a->GetGroundPos().y == b->GetGroundPos().y)
	{
		return a->GetGroundPos().x < b->GetGroundPos().x;
	}
	else
	{
		return a->GetGroundPos().y < b->GetGroundPos().y;
	}
}