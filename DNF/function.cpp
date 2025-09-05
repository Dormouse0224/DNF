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

list<CAlbum*> LoadQueue = {};
list<wstring> ReadQueue = {};
std::mutex queueMutex;
std::mutex queueMutex1;
std::condition_variable loadCV;

void AddLoadQueue(CAlbum* _queue)
{
	std::unique_lock<std::mutex> lock(queueMutex);
	LoadQueue.push_back(_queue);
	loadCV.notify_one();
	lock.unlock();
}

void AddReadQueue(wstring _queue)
{
	std::unique_lock<std::mutex> lock1(queueMutex1);
	ReadQueue.push_back(_queue);
	lock1.unlock();
}