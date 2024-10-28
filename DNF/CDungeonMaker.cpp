#include "pch.h"
#include "CDungeonMaker.h"
#include "CButton.h"
#include "CEngine.h"
#include "CAlbumPlayer.h"
#include "DialogProc.h"
#include "resource.h"
#include "CTextureMgr.h"
#include "CKeyMgr.h"
#include "CCameraMgr.h"
#include "CLevelMgr.h"
#include "CSound.h"
#include "CSoundMgr.h"
#include "CTextMgr.h"

CDungeonMaker::CDungeonMaker()
	: CLevel(L"DungeonMaker")
	, m_StageInfoMap{}
	, m_SelectedTile(-1, -1)
	, m_RightBound(820)
	, m_bFinalExist(false)
{
}

CDungeonMaker::~CDungeonMaker()
{
	ClearStageInfoMap();
}

void CDungeonMaker::Begin()
{
	// 카메라 위치 초기화
	CCameraMgr::GetInst()->InitCameraPos();

	// 타일 선택 상태 초기화
	m_SelectedTile = Vec2D(-1, -1);

	// DungeonMaker 레벨 버튼 추가
	CButton* pAddStageBTN = new CButton(L"AddStage");
	pAddStageBTN->SetDelegate(this, (DELEGATE_0)&CDungeonMaker::CreateStageCallback);
	pAddStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"AddStage_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\AddStage_Idle.animation"), BtnState::IDLE);
	pAddStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"AddStage_CursorOn"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\AddStage_CursorOn.animation"), BtnState::CURSOR_ON);
	pAddStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"AddStage_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\AddStage_Pressed.animation"), BtnState::PRESSED);
	AddObject(pAddStageBTN, LayerType::UI);
	pAddStageBTN->SetScale(Vec2D(219, 47));
	pAddStageBTN->SetLocation(Vec2D(833, 10));

	CButton* pEditStageBTN = new CButton(L"EditStage");
	pEditStageBTN->SetDelegate(this, (DELEGATE_0)&CDungeonMaker::EditStageCallback);
	pEditStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"EditStage_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\EditStage_Idle.animation"), BtnState::IDLE);
	pEditStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"EditStage_CursorOn"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\EditStage_CursorOn.animation"), BtnState::CURSOR_ON);
	pEditStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"EditStage_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\EditStage_Pressed.animation"), BtnState::PRESSED);
	AddObject(pEditStageBTN, LayerType::UI);
	pEditStageBTN->SetScale(Vec2D(219, 47));
	pEditStageBTN->SetLocation(Vec2D(833, 65));

	CButton* pDeleteStageBTN = new CButton(L"DeleteStage");
	pDeleteStageBTN->SetDelegate(this, (DELEGATE_0)&CDungeonMaker::DeleteStageCallback);
	pDeleteStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"DeleteStage_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\DeleteStage_Idle.animation"), BtnState::IDLE);
	pDeleteStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"DeleteStage_CursorOn"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\DeleteStage_CursorOn.animation"), BtnState::CURSOR_ON);
	pDeleteStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"DeleteStage_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\DeleteStage_Pressed.animation"), BtnState::PRESSED);
	AddObject(pDeleteStageBTN, LayerType::UI);
	pDeleteStageBTN->SetScale(Vec2D(219, 47));
	pDeleteStageBTN->SetLocation(Vec2D(833, 120));

	CButton* pToggleFinalBTN = new CButton(L"ToggleFinal");
	pToggleFinalBTN->SetDelegate(this, (DELEGATE_0)&CDungeonMaker::ToggleFinalCallback);
	pToggleFinalBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ToggleFinal_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ToggleFinal_Idle.animation"), BtnState::IDLE);
	pToggleFinalBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ToggleFinal_CursorOn"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ToggleFinal_CursorOn.animation"), BtnState::CURSOR_ON);
	pToggleFinalBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ToggleFinal_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ToggleFinal_Pressed.animation"), BtnState::PRESSED);
	AddObject(pToggleFinalBTN, LayerType::UI);
	pToggleFinalBTN->SetScale(Vec2D(219, 47));
	pToggleFinalBTN->SetLocation(Vec2D(833, 430));

	CButton* pSaveDungeonBTN = new CButton(L"SaveDungeon");
	pSaveDungeonBTN->SetDelegate(this, (DELEGATE_0)&CDungeonMaker::SaveDungeonCallback);
	pSaveDungeonBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"SaveDungeon_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\SaveDungeon_Idle.animation"), BtnState::IDLE);
	pSaveDungeonBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"SaveDungeon_CursorOn"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\SaveDungeon_CursorOn.animation"), BtnState::CURSOR_ON);
	pSaveDungeonBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"SaveDungeon_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\SaveDungeon_Pressed.animation"), BtnState::PRESSED);
	AddObject(pSaveDungeonBTN, LayerType::UI);
	pSaveDungeonBTN->SetScale(Vec2D(219, 47));
	pSaveDungeonBTN->SetLocation(Vec2D(833, 485));

	CButton* pLoadDungeonBTN = new CButton(L"LoadDungeon");
	pLoadDungeonBTN->SetDelegate(this, (DELEGATE_0)&CDungeonMaker::LoadDungeonCallback);
	pLoadDungeonBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"LoadDungeon_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\LoadDungeon_Idle.animation"), BtnState::IDLE);
	pLoadDungeonBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"LoadDungeon_CursorOn"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\LoadDungeon_CursorOn.animation"), BtnState::CURSOR_ON);
	pLoadDungeonBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"LoadDungeon_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\LoadDungeon_Pressed.animation"), BtnState::PRESSED);
	AddObject(pLoadDungeonBTN, LayerType::UI);
	pLoadDungeonBTN->SetScale(Vec2D(219, 47));
	pLoadDungeonBTN->SetLocation(Vec2D(833, 540));

	// 던전메이커 레벨 BGM 세팅
	CSound* pStartBGM = CSoundMgr::GetInst()->GetSound(L"DungeonMakerBGM", L"\\music\\jelva.ogg");
	pStartBGM->PlayToBGM(true);

}

void CDungeonMaker::Tick()
{
	// 메뉴 영역을 제외한 영역에서 그리드의 칸을 클릭 시
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::PRESSED && CKeyMgr::GetInst()->GetMousePos() < Vec2D(m_RightBound, 600))
	{
		Vec2D MousePos = CKeyMgr::GetInst()->GetMousePos() + CCameraMgr::GetInst()->GetCameraPos();
		if (MousePos >> Vec2D(0, 0) && MousePos << Vec2D(1000, 1000))
		{
			m_SelectedTile = Vec2D((int)(MousePos.x / 100), (int)(MousePos.y / 100));
		}
	}

	// ESC 누르면 시작레벨로
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::ESC) == Key_state::TAP)
	{
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"Level_Start"));
	}
	
	CLevel::Tick();
}

void CDungeonMaker::FinalTick()
{
	CLevel::FinalTick();
}

void CDungeonMaker::Render()
{
	// 그리드
	int row = 10;
	int col = 10;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			CTextureMgr::GetInst()->DrawRect(Color(255, 120, 255, 120), 3, Vec2D(i * 100, j * 100), Vec2D(100, 100));
		}
	}

	// 작성된 스테이지
	for (map<Vec2D, StageInfo*>::iterator iter = m_StageInfoMap.begin(); iter != m_StageInfoMap.end(); ++iter)
	{
		CTextureMgr::GetInst()->FillRect(Color(255, 0, 255, 255), iter->first * 100, Vec2D(100, 100));
		CTextureMgr::GetInst()->DrawRect(Color(255, 51, 153, 255), 3, iter->first * 100, Vec2D(100, 100));
		if (iter->second->StageType == StageType::START)
		{
			CTextMgr::GetInst()->Render((UINT)(iter->first.x * 100), (UINT)(iter->first.y * 100), L"Start", Color(0, 0, 0));
		}
		else if (iter->second->StageType == StageType::FINAL)
		{
			CTextMgr::GetInst()->Render((UINT)(iter->first.x * 100), (UINT)(iter->first.y * 100), L"Final", Color(0, 0, 0));
		}
	}

	// 선택된 칸
	if (m_SelectedTile != Vec2D(-1, -1))
	{
		CTextureMgr::GetInst()->DrawRect(Color(255, 255, 120, 120), 3, m_SelectedTile * 100, Vec2D(100, 100));
	}

	// 메뉴 영역 배경
	CTextureMgr::GetInst()->FillRect(Color(210, 0, 0, 0), Vec2D(m_RightBound, 0), Vec2D(CEngine::GetInst()->GetResolution().x - m_RightBound, 600.f), true);


	CLevel::Render();
}

void CDungeonMaker::End()
{
	ClearAll();
}


void CDungeonMaker::AddStageInfo(Vec2D _GridLocation, StageInfo* _stageinfo)
{
	assert(m_StageInfoMap.insert(make_pair(_GridLocation, _stageinfo)).second);
	_stageinfo->GridLoc = _GridLocation;
}

StageInfo* CDungeonMaker::FindStageInfo(Vec2D _GridLocation)
{
	map<Vec2D, StageInfo*>::iterator iter = m_StageInfoMap.find(_GridLocation);
	if (iter == m_StageInfoMap.end())
		return nullptr;
	else
		return iter->second;
}


void CDungeonMaker::ClearStageInfoMap()
{
	for (map<Vec2D, StageInfo*>::iterator iter = m_StageInfoMap.begin(); iter != m_StageInfoMap.end(); ++iter)
	{
		if (iter->second != nullptr)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}
	m_StageInfoMap.clear();
}

void CDungeonMaker::CreateStageCallback()
{
	// Final 이 지정되어있는경우 생성 불가
	if (m_bFinalExist)
	{
		MessageBox(CEngine::GetInst()->GetMainWnd(), L"Final 스테이지가 지정되어 있습니다.\n해제 후 추가해주세요.", L"타일 오류", MB_ICONWARNING | MB_OK);
		return;
	}

	// 이미 스테이지 타일이 할당된 위치인 경우 생성 불가
	if (m_StageInfoMap.find(m_SelectedTile) != m_StageInfoMap.end())
	{
		MessageBox(CEngine::GetInst()->GetMainWnd(), L"이미 스테이지가 할당되어 있습니다.\n제거 후 추가해주세요.", L"타일 오류", MB_ICONWARNING | MB_OK);
		return;
	}

	// 스테이지가 하나도 없는 경우 시작 스테이지 생성
	if (m_StageInfoMap.empty())
	{
		if (m_SelectedTile != Vec2D(-1, -1))
		{
			// 스테이지 생성
			DialogBox(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AddStage), CEngine::GetInst()->GetMainWnd(), &AddStageProc);

			// 해당 타일에 스테이지가 정상적으로 추가된 경우, 스테이지를 시작 스테이지로 세팅
			map<Vec2D, StageInfo*>::iterator iter = m_StageInfoMap.find(m_SelectedTile);
			if (iter != m_StageInfoMap.end())
			{
				iter->second->StageType = StageType::START;
			}
		}
		else
		{
			MessageBox(CEngine::GetInst()->GetMainWnd(), L"선택된 타일이 없습니다.", L"타일 오류", MB_ICONWARNING | MB_OK);
		}

	}
	else
	{
		// 일반 스테이지 생성 - 연결된 타일에만 생성할 수 있음
		// 연결된 타일을 탐색할 방향 배열
		pair<PortalDirection, Vec2D> directions[] = 
		{
			{PortalDirection::BOTTOM, Vec2D(0, -1)},	// 위쪽
			{PortalDirection::TOP, Vec2D(0, 1)},		// 아래쪽
			{PortalDirection::RIGHT, Vec2D(-1, 0)},		// 왼쪽
			{PortalDirection::LEFT, Vec2D(1, 0)}		// 오른쪽
		};

		// 연결된 타일이 있는지 확인
		bool hasConnectedTile = false;

		for (const pair<PortalDirection, Vec2D>& direction : directions)
		{
			Vec2D offset = direction.second;
			map<Vec2D, StageInfo*>::iterator iter = m_StageInfoMap.find(m_SelectedTile + offset);

			if (iter != m_StageInfoMap.end()) 
			{
				hasConnectedTile = true;
				break;
			}
		}

		// 연결된 타일이 하나라도 있으면 스테이지 생성
		if (hasConnectedTile)
		{
			// 스테이지 생성
			DialogBox(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AddStage), CEngine::GetInst()->GetMainWnd(), &AddStageProc);

			// 스테이지가 추가된 경우 스테이지를 중간 스테이지로 세팅
			map<Vec2D, StageInfo*>::iterator middleStage = m_StageInfoMap.find(m_SelectedTile);
			if (middleStage != m_StageInfoMap.end())
			{
				middleStage->second->StageType = StageType::MIDDLE;

				// 포탈 정보 세팅
				for (const pair<PortalDirection, Vec2D>& direction : directions)
				{
					Vec2D offset = direction.second;
					map<Vec2D, StageInfo*>::iterator targetStage = m_StageInfoMap.find(m_SelectedTile + offset);
					if (targetStage != m_StageInfoMap.end())
					{
						targetStage->second->arrPortalInfo[(int)direction.first].PointStageName = middleStage->second->StageName;
						middleStage->second->arrPortalInfo[3 - (int)direction.first].PointStageName = targetStage->second->StageName;
					}
				}
			}
		}
		else
		{
			MessageBox(CEngine::GetInst()->GetMainWnd(), L"연결되는 타일이 없습니다.", L"타일 오류", MB_ICONWARNING | MB_OK);
		}
	}
}

void CDungeonMaker::EditStageCallback()
{
	// 선택된 스테이지 정보를 바탕으로 스테이지 메이커로 이동
	if (m_StageInfoMap.find(m_SelectedTile) != m_StageInfoMap.end())
	{
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"StageMaker"));
	}
}

void CDungeonMaker::DeleteStageCallback()
{
	map<Vec2D, StageInfo*>::iterator iter = m_StageInfoMap.find(m_SelectedTile);
	// 선택된 타일에 스테이지가 존자하는 경우 삭제
	if (iter != m_StageInfoMap.end())
	{
		// 스타트 스테이지가 마지막에 삭제되어야함
		if (iter->second->StageType == StageType::START && m_StageInfoMap.size() > 1)
			return;


		// 삭제되는 스테이지 주변의 연결된 스테이지 포탈을 제거
		pair<PortalDirection, Vec2D> directions[] =
		{
			{PortalDirection::BOTTOM, Vec2D(0, -1)},	// 위쪽
			{PortalDirection::TOP, Vec2D(0, 1)},		// 아래쪽
			{PortalDirection::RIGHT, Vec2D(-1, 0)},		// 왼쪽
			{PortalDirection::LEFT, Vec2D(1, 0)}		// 오른쪽
		};

		for (const pair<PortalDirection, Vec2D>& direction : directions)
		{
			Vec2D offset = direction.second;
			map<Vec2D, StageInfo*>::iterator targetStage = m_StageInfoMap.find(m_SelectedTile + offset);
			if (targetStage != m_StageInfoMap.end())
			{
				targetStage->second->arrPortalInfo[(int)direction.first].PointStageName = L"";
			}
		}

		// 스테이지 메모리 해제 후 삭제
		if (iter->second)
		{
			delete iter->second;
			iter->second = nullptr;
		}
		m_StageInfoMap.erase(m_SelectedTile);
	}
}

void CDungeonMaker::ToggleFinalCallback()
{
	// 선택된 스테이지의 MIDDLE, FINAL 을 토글
	map<Vec2D, StageInfo*>::iterator stageIter = m_StageInfoMap.find(m_SelectedTile);
	if (stageIter != m_StageInfoMap.end())
	{
		if (stageIter->second->StageType == StageType::MIDDLE && !m_bFinalExist)
		{
			stageIter->second->StageType = StageType::FINAL;
			m_bFinalExist = true;
		}
		else if (stageIter->second->StageType == StageType::FINAL)
		{
			stageIter->second->StageType = StageType::MIDDLE;
			m_bFinalExist = false;
		}
	}
}

void CDungeonMaker::SaveDungeonCallback()
{
	// 던전과 스테이지 정보를 파일로 저장
	WCHAR filepath[255] = {};
	WCHAR filename[255] = {};
	wstring wstr = CEngine::GetInst()->GetResourcePathW() + L"\\dungeon";
	OPENFILENAME Desc = {};
	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = CEngine::GetInst()->GetMainWnd();
	Desc.lpstrFilter = L"dungeon\0*.dungeon\0ALL\0*.*\0";
	Desc.lpstrFile = filepath;
	Desc.nMaxFile = 255;
	Desc.lpstrFileTitle = filename;
	Desc.nMaxFileTitle = 255;
	Desc.lpstrInitialDir = wstr.c_str();
	Desc.lpstrTitle = L"던전 파일 저장";
	Desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	Desc.lpstrDefExt = L"dungeon";

	if (GetSaveFileName(&Desc))
	{
		int StageCount = m_StageInfoMap.size();

		// 던전의 스테이지 헤더 작성
		vector<StageInfoHeader> HeaderVec;
		for (map<Vec2D, StageInfo*>::iterator iter = m_StageInfoMap.begin(); iter != m_StageInfoMap.end(); ++iter)
		{
			StageInfoHeader header;
			header.StageType = iter->second->StageType;
			header.StageSize = iter->second->StageSize;
			header.GridLoc = iter->second->GridLoc;
			header.UpperBound = iter->second->UpperBound;
			header.vecBGACount = iter->second->vecBGA.size();
			header.vecMonsterInfoCount = iter->second->vecMonsterInfo.size();
			header.vecWallInfoCount = iter->second->vecWallInfo.size();
			header.vecNPCInfoCount = iter->second->vecNPCInfo.size();
			HeaderVec.push_back(header);
		}

		ofstream DungeonFile;
		DungeonFile.open(filepath, ios::binary);
		// 스테이지 개수
		DungeonFile.write((char*)&StageCount, sizeof(StageCount));
		// 스테이지 헤더
		for (int i = 0; i < StageCount; ++i)
		{
			DungeonFile.write((char*)&HeaderVec[i], sizeof(HeaderVec[i]));
		}
		// 스테이지 데이터
		for (map<Vec2D, StageInfo*>::iterator iter = m_StageInfoMap.begin(); iter != m_StageInfoMap.end(); ++iter)
		{
			// 스테이지 이름, BGM 정보
			int stageNameLen = iter->second->StageName.size();
			wstring stageName = iter->second->StageName;
			int bgmPathLen = iter->second->BGMPath.size();
			wstring bgmPath = iter->second->BGMPath;
			DungeonFile.write((char*)&stageNameLen, sizeof(stageNameLen));
			DungeonFile.write((char*)stageName.c_str(), stageNameLen * sizeof(wchar_t));
			DungeonFile.write((char*)&bgmPathLen, sizeof(bgmPathLen));
			DungeonFile.write((char*)bgmPath.c_str(), bgmPathLen * sizeof(wchar_t));
			// BGA
			for (int i = 0; i < iter->second->vecBGA.size(); ++i)
			{
				int wstrLen = iter->second->vecBGA[i].size();
				wstring wstr = iter->second->vecBGA[i];
				DungeonFile.write((char*)&wstrLen, sizeof(wstrLen));
				DungeonFile.write((char*)wstr.c_str(), wstrLen * sizeof(wchar_t));
			}
			// 포탈
			for (int i = 0; i < 4; ++i)
			{
				Vec2D vec = iter->second->arrPortalInfo[i].Location;
				int wstrLen = iter->second->arrPortalInfo[i].PointStageName.size();
				wstring wstr = iter->second->arrPortalInfo[i].PointStageName;
				DungeonFile.write((char*)&vec, sizeof(vec));
				DungeonFile.write((char*)&wstrLen, sizeof(wstrLen));
				DungeonFile.write((char*)wstr.c_str(), wstrLen * sizeof(wchar_t));
			}
			// 몬스터
			for (int i = 0; i < iter->second->vecMonsterInfo.size(); ++i)
			{
				int wstrLen = iter->second->vecMonsterInfo[i]->Name.size();
				wstring wstr = iter->second->vecMonsterInfo[i]->Name;
				MonsterTemplate monsterTemplate = iter->second->vecMonsterInfo[i]->MonsterTemplate;
				Vec2D vec = iter->second->vecMonsterInfo[i]->pos;
				DungeonFile.write((char*)&wstrLen, sizeof(wstrLen));
				DungeonFile.write((char*)wstr.c_str(), wstrLen * sizeof(wchar_t));
				DungeonFile.write((char*)&monsterTemplate, sizeof(monsterTemplate));
				DungeonFile.write((char*)&vec, sizeof(vec));
			}
			// 벽
			for (int i = 0; i < iter->second->vecWallInfo.size(); ++i)
			{
				int wstrLen = iter->second->vecWallInfo[i]->Name.size();
				wstring wstr = iter->second->vecWallInfo[i]->Name;
				Vec2D vec = iter->second->vecWallInfo[i]->Pos;
				Vec2D vec1 = iter->second->vecWallInfo[i]->Size;
				DungeonFile.write((char*)&wstrLen, sizeof(wstrLen));
				DungeonFile.write((char*)wstr.c_str(), wstrLen * sizeof(wchar_t));
				DungeonFile.write((char*)&vec, sizeof(vec));
				DungeonFile.write((char*)&vec1, sizeof(vec1));
			}
			// NPC
			for (int i = 0; i < iter->second->vecNPCInfo.size(); ++i)
			{
				int wstrLen = iter->second->vecNPCInfo[i]->Name.size();
				wstring wstr = iter->second->vecNPCInfo[i]->Name;
				Vec2D vec = iter->second->vecNPCInfo[i]->Pos;
				Vec2D vec1 = iter->second->vecNPCInfo[i]->Size;
				int wstrLen1 = iter->second->vecNPCInfo[i]->IdleAnimation.size();
				wstring wstr1 = iter->second->vecNPCInfo[i]->IdleAnimation;
				DungeonFile.write((char*)&wstrLen, sizeof(wstrLen));
				DungeonFile.write((char*)wstr.c_str(), wstrLen * sizeof(wchar_t));
				DungeonFile.write((char*)&vec, sizeof(vec));
				DungeonFile.write((char*)&vec1, sizeof(vec1));
				DungeonFile.write((char*)&wstrLen1, sizeof(wstrLen1));
				DungeonFile.write((char*)wstr1.c_str(), wstrLen1 * sizeof(wchar_t));
			}
		}
		DungeonFile.close();
		MessageBox(CEngine::GetInst()->GetMainWnd(), L"저장되었습니다.", L"알림", MB_ICONINFORMATION | MB_OK);
	}
}

void CDungeonMaker::LoadDungeonCallback()
{
	ClearStageInfoMap();

	// 파일로부터 던전 정보를 불러와 로드
	WCHAR filepath[255] = {};
	WCHAR filename[255] = {};
	wstring wstr = CEngine::GetInst()->GetResourcePathW() + L"\\dungeon";
	OPENFILENAME Desc = {};
	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = CEngine::GetInst()->GetMainWnd();
	Desc.lpstrFilter = L"dungeon\0*.dungeon\0ALL\0*.*\0";
	Desc.lpstrFile = filepath;
	Desc.nMaxFile = 255;
	Desc.lpstrFileTitle = filename;
	Desc.nMaxFileTitle = 255;
	Desc.lpstrInitialDir = wstr.c_str();
	Desc.lpstrTitle = L"던전 파일 로드";
	Desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	Desc.lpstrDefExt = L"dungeon";

	if (GetOpenFileName(&Desc))
	{
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

			m_StageInfoMap.insert(make_pair(pStageInfo->GridLoc, pStageInfo));
			int a = 0;
		}
		DungeonFile.close();
	}
}
