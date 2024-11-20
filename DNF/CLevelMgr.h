#pragma once

#include "CLevel.h"

class CLevelMgr
{
	SINGLETON(CLevelMgr);

private:
	map<wstring, CLevel*> m_CLevels;
	CLevel* m_CurrentLevel;

	vector<wstring> m_DungeonVec;

	PlayerInfo* m_PlayerInfo;

	map<wstring, DungeonInfo*> m_DungeonMap;

public:
	void Init();
	void AddLevel(wstring _LevelName, CLevel* _CLevel);
	CLevel* FindLevel(wstring _LevelName);
	DungeonInfo* FindDungeon(wstring _DungeonName);
	void Progress();
	void Render();
	
	CLevel* GetCurrentLevel() { return m_CurrentLevel; }
	void SetCurrentLevel(CLevel* _level) { m_CurrentLevel = _level; }
	void ChangeLevel(CLevel* _Dest);

	void ReadDungeonList();
	vector<wstring> GetDungeonVec() { return m_DungeonVec; }
	void LoadStage(wstring _fileName, map<Vec2D, StageInfo*>& _StageInfoMap);

	PlayerInfo* GetPlayerInfo() { return m_PlayerInfo; }
	void SetPlayerInfo(PlayerInfo* _info) { m_PlayerInfo = _info; }

};

