#pragma once
class CTaskMgr
{
	SINGLETON(CTaskMgr);

private:
	vector<StTask> m_TaskQueue;
	vector<CObj*> m_Garbage;
 
public:
	void AddTask(StTask _task) { m_TaskQueue.push_back(_task); }
	void Tick();

};

