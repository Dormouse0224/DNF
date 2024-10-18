#include "pch.h"
#include "CDungeonMaker.h"
#include "CButton.h"
#include "CEngine.h"
#include "CAlbumPlayer.h"

CDungeonMaker::CDungeonMaker(wstring _name)
	: CLevel(_name)
{
}

CDungeonMaker::~CDungeonMaker()
{
}

void CDungeonMaker::Begin()
{
	CButton* pCreateStageBTN = new CButton(L"CreateStage");
	pCreateStageBTN->SetDelegate(this, (DELEGATE_0)&CDungeonMaker::CreateStageCallback);
	pCreateStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"LoadNPK_Idle"
		, CEngine::GetInst()->GetResourcePathA() + "\\animation\\LoadNPK_Idle.animation"), BtnState::IDLE);
	pCreateStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"LoadNPK_CursorOn"
		, CEngine::GetInst()->GetResourcePathA() + "\\animation\\LoadNPK_CursorOn.animation"), BtnState::CURSOR_ON);
	pCreateStageBTN->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"LoadNPK_Pressed"
		, CEngine::GetInst()->GetResourcePathA() + "\\animation\\LoadNPK_Pressed.animation"), BtnState::PRESSED);
	AddObject(pCreateStageBTN, LayerType::UI);
	pCreateStageBTN->SetScale(Vec2D(219, 47));
	pCreateStageBTN->SetLocation(Vec2D(830, 10));
}

void CDungeonMaker::Tick()
{
}

void CDungeonMaker::FinalTick()
{
}

void CDungeonMaker::Render()
{
}

void CDungeonMaker::End()
{
}

void CDungeonMaker::CreateStageCallback()
{
}
