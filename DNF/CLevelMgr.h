#pragma once

#include "CLevel.h"

class CLevelMgr
{
	SINGLETON(CLevelMgr);

private:
	map<wstring, CLevel*> m_CLevels;
	CLevel* m_CurrentLevel;

	map<wstring, DungeonInfo*> m_DungeonMap;

public:
	void Init();
	void AddLevel(wstring _LevelName, CLevel* _CLevel) { assert(m_CLevels.insert(make_pair(_LevelName, _CLevel)).second); }
	CLevel* FindLevel(wstring _LevelName);
	DungeonInfo* FindDungeon(wstring _DungeonName);
	void Progress();
	void Render();
	
	CLevel* GetCurrentLevel() { return m_CurrentLevel; }
	void SetCurrentLevel(CLevel* _level) { m_CurrentLevel = _level; }
	void ChangeLevel(CLevel* _Dest);

	void ReadDungeonList();
	void LoadStage(wstring _fileName, map<Vec2D, StageInfo*>& _StageInfoMap);
};

