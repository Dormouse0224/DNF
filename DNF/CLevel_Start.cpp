#include "pch.h"
#include "CLevel_Start.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CTextMgr.h"
#include "CKeyMgr.h"
#include "CLevelMgr.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CEngine.h"
#include "CCameraMgr.h"
#include "CButton.h"
#include "CTextureMgr.h"

CLevel_Start::CLevel_Start()
	:CLevel(L"Level_Start")
{
}

CLevel_Start::~CLevel_Start()
{
}

void CLevel_Start::Begin()
{
	AddReadQueue(L"AvatarResources.txt");

	// 카메라 위치 초기화
	CCameraMgr::GetInst()->InitCameraPos();

	// 시작 레벨의 배경 생성
	CBackground* pStartBackground = new CBackground(L"StartBackground");

	pStartBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"temp", CEngine::GetInst()->GetResourcePathW() + L"\\animation\\start_background.animation"));
	pStartBackground->SetScale(Vec2D(1066, 600));
	AddObject(pStartBackground, LayerType::Background);

	// 게임 시작 버튼 추가
	CButton* GameStartBtn = new CButton(L"Btn_GameStart");
	GameStartBtn->SetDelegate0(this, (DELEGATE_0)&CLevel_Start::GameStartBtnCallback);
	GameStartBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"GameStart_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\GameStart_Idle.animation"), BtnState::IDLE);
	GameStartBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"GameStart_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\GameStart_Pressed.animation"), BtnState::CURSOR_ON);
	GameStartBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"GameStart_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\GameStart_Pressed.animation"), BtnState::PRESSED);
	AddObject(GameStartBtn, LayerType::UI);
	GameStartBtn->SetScale(Vec2D(304, 38));
	GameStartBtn->SetUIPos(Vec2D(610, 360));

	// 텍스처 에디터 버튼 추가
	CButton* TextureEditBtn = new CButton(L"Btn_TextureEdit");
	TextureEditBtn->SetDelegate0(this, (DELEGATE_0)&CLevel_Start::TextureEditBtnCallback);
	TextureEditBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"TextureEdit_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\TextureEdit_Idle.animation"), BtnState::IDLE);
	TextureEditBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"TextureEdit_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\TextureEdit_Pressed.animation"), BtnState::CURSOR_ON);
	TextureEditBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"TextureEdit_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\TextureEdit_Pressed.animation"), BtnState::PRESSED);
	AddObject(TextureEditBtn, LayerType::UI);
	TextureEditBtn->SetScale(Vec2D(333, 38));
	TextureEditBtn->SetUIPos(Vec2D(610, 430));

	// 던전 메이커 버튼 추가
	CButton* DungeonMakerBtn = new CButton(L"Btn_DungeonMaker");
	DungeonMakerBtn->SetDelegate0(this, (DELEGATE_0)&CLevel_Start::DungeonMakerBtnCallback);
	DungeonMakerBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"DungeonMaker_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\DungeonMaker_Idle.animation"), BtnState::IDLE);
	DungeonMakerBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"DungeonMaker_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\DungeonMaker_Pressed.animation"), BtnState::CURSOR_ON);
	DungeonMakerBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"DungeonMaker_Pressed"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\DungeonMaker_Pressed.animation"), BtnState::PRESSED);
	AddObject(DungeonMakerBtn, LayerType::UI);
	DungeonMakerBtn->SetScale(Vec2D(421, 38));
	DungeonMakerBtn->SetUIPos(Vec2D(610, 500));


	// 리소스 미리 로딩
	CTextureMgr::PreloadFromFile(L"SystemResources.txt");

	// 시작 레벨 BGM 세팅
	CSound* pStartBGM = CSoundMgr::GetInst()->GetSound(L"StartBGM", L"\\music\\character_stage.ogg");
	pStartBGM->PlayToBGM(true);

	CLevel::Begin();
}

void CLevel_Start::Tick()
{

	CLevel::Tick();

}

void CLevel_Start::FinalTick()
{
	CLevel::FinalTick();
}

void CLevel_Start::Render()
{
	CLevel::Render();
}

void CLevel_Start::End()
{
	ClearAll();
}

void CLevel_Start::GameStartBtnCallback()
{
	CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"SeriaRoom"));
}

void CLevel_Start::TextureEditBtnCallback()
{
	CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"Level_Edit"));
}

void CLevel_Start::DungeonMakerBtnCallback()
{
	CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"DungeonMaker"));
}
