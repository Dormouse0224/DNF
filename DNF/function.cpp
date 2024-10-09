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
