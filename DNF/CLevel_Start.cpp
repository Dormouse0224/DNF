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

CLevel_Start::CLevel_Start()
	:CLevel(L"Level_Start")
{
}

CLevel_Start::~CLevel_Start()
{
}

void CLevel_Start::Begin()
{
	// ī�޶� ��ġ �ʱ�ȭ
	CCameraMgr::GetInst()->InitCameraPos();

	// ���� ������ ��� ����
	CBackground* pStartBackground = new CBackground(L"StartBackground");

	pStartBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"temp", CEngine::GetInst()->GetResourcePathA() + "\\animation\\start_background.animation"));
	pStartBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"temp", CEngine::GetInst()->GetResourcePathA() + "\\animation\\archer_walk_30.animation"));
	pStartBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"temp", CEngine::GetInst()->GetResourcePathA() + "\\animation\\archer_walk_-30.animation"));
	pStartBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"temp", CEngine::GetInst()->GetResourcePathA() + "\\animation\\archer_walk_0.animation"));
	pStartBackground->SetScale(Vec2D(1066, 600));
	AddObject(pStartBackground, LayerType::Background);

	// ���� ���� BGM ����
	CSound* pStartBGM = CSoundMgr::GetInst()->GetSound(L"StartBGM", L"\\music\\character_stage.ogg");
	pStartBGM->PlayToBGM(true);

	CLevel::Begin();
}

void CLevel_Start::Tick()
{
	// CTRL + E ������ ������ ������
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::LCTRL) == Key_state::PRESSED && CKeyMgr::GetInst()->GetKeyState(Keyboard::E) == Key_state::PRESSED)
	{
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"Level_Edit"));
	}

	// CTRL + D ������ ��������Ŀ ������
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::LCTRL) == Key_state::PRESSED && CKeyMgr::GetInst()->GetKeyState(Keyboard::D) == Key_state::PRESSED)
	{
		CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(L"DungeonMaker"));
	}


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
	ClearObject();
}
