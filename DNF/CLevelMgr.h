#pragma once

#include "CLevel.h"

class CLevelMgr
{
	SINGLETON(CLevelMgr);

private:
	map<wstring, CLevel*> m_CLevels;
	CLevel* m_CurrentLevel;

public:
	void Init();
	void AddLevel(wstring _LevelName, CLevel* _CLevel) { m_CLevels.insert(make_pair(_LevelName, _CLevel)); }
	CLevel* FindLevel(wstring _LevelName);
	void Progress();
	void Render();
	
	CLevel* GetCurrentLevel() { return m_CurrentLevel; }
};

