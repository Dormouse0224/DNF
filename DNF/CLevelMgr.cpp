#include "pch.h"
#include "CLevelMgr.h"
#include "CDbgRender.h"
#include "CTexture.h"
#include "CBackground.h"
#include "CEngine.h"
#include "CNpkMgr.h"
#include "CTaskMgr.h"
#include "CTimeMgr.h"

#include "CLevel_Start.h"
#include "CLevel_Edit.h"
#include "CDungeonMaker.h"
#include "CStageMaker.h"
#include "CPurpleSeaRoad.h"
#include "CCliff.h"
#include "CS01.h"
#include "CS03.h"

#include "CAlbum.h"
#include "CAlbumPlayer.h"

CLevelMgr::CLevelMgr()
	: m_CurrentLevel(nullptr)
	, m_CLevels()
	, m_DungeonMap()
{

}

CLevelMgr::~CLevelMgr()
{
	for (map<wstring, CLevel*>::iterator iter = m_CLevels.begin(); iter != m_CLevels.end(); ++iter)
	{
		if (iter->second != nullptr)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}
}

void CLevelMgr::Init()
{
	// 초기 레벨 생성 및 추가

	CLevel_Start* pStartLevel = new CLevel_Start;
	CLevel_Edit* pEditLevel = new CLevel_Edit;
	CDungeonMaker* pDungeonMaker = new CDungeonMaker;
	CStageMaker* pStageMaker = new CStageMaker;
	CPurpleSeaRoad* pPurpleSeaRoad = new CPurpleSeaRoad;
	CCliff* pCliff = new CCliff;
	CS01* pS01 = new CS01;
	CS03* pS03 = new CS03;
	ReadDungeonList();
	m_CurrentLevel = pPurpleSeaRoad;
	m_CurrentLevel->Begin();
}

CLevel* CLevelMgr::FindLevel(wstring _LevelName)
{
	map<wstring, CLevel*>::iterator iter = m_CLevels.find(_LevelName);
	if (iter == m_CLevels.end())
		return nullptr;
	else
		return iter->second;
}

DungeonInfo* CLevelMgr::FindDungeon(wstring _DungeonName)
{
	map<wstring, DungeonInfo*>::iterator iter = m_DungeonMap.find(_DungeonName);
	if (iter == m_DungeonMap.end())
		return nullptr;
	else
		return iter->second;
}

void CLevelMgr::Progress()
{
	if (nullptr == m_CurrentLevel)
		return;

	m_CurrentLevel->Tick();
	m_CurrentLevel->FinalTick();
}

void CLevelMgr::Render()
{
	m_CurrentLevel->Render();

	if (CDbgRender::GetInst()->GetShowState())
	{
		CDbgRender::GetInst()->Render();
	}
}

void CLevelMgr::ChangeLevel(CLevel* _Dest)
{
	StTask changeLevel;
	changeLevel.m_TaskType = TaskType::ChangeLevel;
	changeLevel.m_param0 = (DWORD_PTR)_Dest;
	CTaskMgr::GetInst()->AddTask(changeLevel);
}

void CLevelMgr::ReadDungeonList()
{
	std::wifstream DungeonList(CEngine::GetInst()->GetResourcePathW() + L"\\dungeon\\_List.txt");
	wstring line;
	map<Vec2D, StageInfo*> StageMap;
	while (std::getline(DungeonList, line)) {
		LoadStage(line, StageMap);
		for (map<Vec2D, StageInfo*>::iterator iter = StageMap.begin(); iter != StageMap.end(); ++iter)
		{
			CStage* newStage = new CStage(iter->second->StageName);
			newStage->SetStageInfo(iter->second);
		}
	}
	DungeonList.close();
}

void CLevelMgr::LoadStage(wstring _fileName, map<Vec2D, StageInfo*>& _StageInfoMap)
{
	wstring filepath = CEngine::GetInst()->GetResourcePathW() + L"\\dungeon\\" + _fileName;

	ifstream DungeonFile;
	DungeonFile.open(filepath, ios::binary);
	int StageCount = 0;
	DungeonFile.read((char*)&StageCount, sizeof(StageCount));

	// 스테이지 헤더
	vector<StageInfoHeader> HeaderVec;
	for (int i = 0; i < StageCount; ++i)
	{
		StageInfoHeader header;
		DungeonFile.read((char*)&header, sizeof(header));
		HeaderVec.push_back(header);
	}
	// 스테이지 데이터
	for (int j = 0; j < HeaderVec.size(); ++j)
	{
		StageInfo* pStageInfo = new StageInfo();
		pStageInfo->StageType = HeaderVec[j].StageType;
		pStageInfo->StageSize = HeaderVec[j].StageSize;
		pStageInfo->GridLoc = HeaderVec[j].GridLoc;
		pStageInfo->UpperBound = HeaderVec[j].UpperBound;

		// 스테이지 이름, BGM 정보
		int stageNameLen = 0;
		WCHAR stageName[255] = {};
		int bgmPathLen = 0;
		WCHAR bgmPath[255] = {};
		DungeonFile.read((char*)&stageNameLen, sizeof(stageNameLen));
		DungeonFile.read((char*)stageName, stageNameLen * sizeof(wchar_t));
		DungeonFile.read((char*)&bgmPathLen, sizeof(bgmPathLen));
		DungeonFile.read((char*)bgmPath, bgmPathLen * sizeof(wchar_t));
		pStageInfo->StageName = stageName;
		pStageInfo->BGMPath = bgmPath;


		// BGA
		for (int i = 0; i < HeaderVec[j].vecBGACount; ++i)
		{
			int wstrLen = 0;
			WCHAR wstr[255] = {};
			DungeonFile.read((char*)&wstrLen, sizeof(wstrLen));
			DungeonFile.read((char*)wstr, wstrLen * sizeof(wchar_t));
			pStageInfo->vecBGA.push_back(wstr);
		}
		// 포탈
		for (int i = 0; i < 4; ++i)
		{
			Vec2D vec;
			int wstrLen = 0;
			WCHAR wstr[255] = {};
			DungeonFile.read((char*)&vec, sizeof(vec));
			DungeonFile.read((char*)&wstrLen, sizeof(wstrLen));
			DungeonFile.read((char*)wstr, wstrLen * sizeof(wchar_t));
			PortalInfo desc;
			desc.Location = vec;
			desc.PointStageName = wstr;
			pStageInfo->arrPortalInfo[i] = desc;
		}
		// 몬스터
		for (int i = 0; i < HeaderVec[j].vecMonsterInfoCount; ++i)
		{
			int wstrLen = 0;
			WCHAR wstr[255] = {};
			MonsterTemplate monsterTemplate;
			Vec2D vec;
			DungeonFile.read((char*)&wstrLen, sizeof(wstrLen));
			DungeonFile.read((char*)wstr, wstrLen * sizeof(wchar_t));
			DungeonFile.read((char*)&monsterTemplate, sizeof(monsterTemplate));
			DungeonFile.read((char*)&vec, sizeof(vec));
			MonsterInfo* pDesc = new MonsterInfo;
			pDesc->Name = wstr;
			pDesc->MonsterTemplate = monsterTemplate;
			pDesc->pos = vec;
			pStageInfo->vecMonsterInfo.push_back(pDesc);
		}
		// 벽
		for (int i = 0; i < HeaderVec[j].vecWallInfoCount; ++i)
		{
			int wstrLen = 0;
			WCHAR wstr[255] = {};
			Vec2D vec;
			Vec2D vec1;
			DungeonFile.read((char*)&wstrLen, sizeof(wstrLen));
			DungeonFile.read((char*)wstr, wstrLen * sizeof(wchar_t));
			DungeonFile.read((char*)&vec, sizeof(vec));
			DungeonFile.read((char*)&vec1, sizeof(vec1));
			WallInfo* pDesc = new WallInfo;
			pDesc->Name = wstr;
			pDesc->Pos = vec;
			pDesc->Size = vec1;
			pStageInfo->vecWallInfo.push_back(pDesc);
		}
		// NPC
		for (int i = 0; i < HeaderVec[j].vecNPCInfoCount; ++i)
		{
			int wstrLen = 0;
			WCHAR wstr[255] = {};
			Vec2D vec;
			Vec2D vec1;
			int wstrLen1 = 0;
			WCHAR wstr1[255] = {};
			DungeonFile.read((char*)&wstrLen, sizeof(wstrLen));
			DungeonFile.read((char*)wstr, wstrLen * sizeof(wchar_t));
			DungeonFile.read((char*)&vec, sizeof(vec));
			DungeonFile.read((char*)&vec1, sizeof(vec1));
			DungeonFile.read((char*)&wstrLen1, sizeof(wstrLen1));
			DungeonFile.read((char*)wstr1, wstrLen1 * sizeof(wchar_t));
			NPCInfo* pDesc = new NPCInfo;
			pDesc->Name = wstr;
			pDesc->Pos = vec;
			pDesc->Size = vec1;
			pDesc->IdleAnimation = wstr1;
			pStageInfo->vecNPCInfo.push_back(pDesc);
		}

		_StageInfoMap.insert(make_pair(pStageInfo->GridLoc, pStageInfo));
	}
	DungeonFile.close();
}
