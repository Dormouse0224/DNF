#pragma once

class CUI;

class CUIMgr
{
public:
	SINGLETON(CUIMgr);
private:


public:
	void Tick();

	CUI* GetPriorityUI(CUI* _ParentUI);

	void ReleaseCheck(CUI* _UI);


};

