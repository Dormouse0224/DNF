#include "pch.h"
#include "CNPCCallback.h"
#include "CNPC.h"
#include "CLevelMgr.h"
#include "CWindow.h"
#include "CButton.h"
#include "CEngine.h"
#include "CAlbumPlayer.h"


void SelectDungeonCallback(CNPC* _this)
{
	if (_this->GetNPCUI())
		return;

	CWindow* pDungeonListWnd = new CWindow(L"DungeonListWnd");
	vector<wstring> DungeonVec = CLevelMgr::GetInst()->GetDungeonVec();
	pDungeonListWnd->SetUIPos(Vec2D(100, 50));
	pDungeonListWnd->SetWindowColor(Color(255, 0, 0, 0));
	pDungeonListWnd->SetBorderColor(Color(255, 255, 255, 255));
	pDungeonListWnd->SetMovalbe(true);
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pDungeonListWnd, LayerType::UI);

	if (!DungeonVec.empty())
	{
		pDungeonListWnd->SetScale(Vec2D(200, (int)(DungeonVec.size() * 30) + 35));
		for (int i = 0; i < DungeonVec.size(); ++i)
		{
			pDungeonListWnd->SetText(L"Dungeon List");
			pDungeonListWnd->SetTextColor(Color(255, 255, 255, 255));
			CButton* pBtn = new CButton(DungeonVec[i]);
			pBtn->SetUIPos(Vec2D(9, i * 30 + 31));
			pBtn->SetScale(Vec2D(182, 27));
			pBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_selectdungeonbtn_idle"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_selectdungeonbtn_idle.animation"), BtnState::IDLE);
			pBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_selectdungeonbtn_cursoron"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_selectdungeonbtn_cursoron.animation"), BtnState::CURSOR_ON);
			pBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_selectdungeonbtn_idle"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_selectdungeonbtn_idle.animation"), BtnState::PRESSED);
			wstring wstr = DungeonVec[i];
			if (wstr.size() > 16)
			{
				wstr = wstr.substr(0, 16) + L"...";
			}
			pBtn->SetText(wstr);
			pBtn->SetTextColor(Color(255, 255, 255, 255));
			pBtn->SetCallback1((CALLBACK_1)&TeleportBtnCallback, (DWORD_PTR)pBtn);
			pDungeonListWnd->AddChild(pBtn, DungeonVec[i]);
		}
	}
	else
	{
		pDungeonListWnd->SetScale(Vec2D(200, 30));
		pDungeonListWnd->SetText(L"No Dungeon Exist.");
		pDungeonListWnd->SetTextColor(Color(255, 255, 255, 255));
	}
	_this->SetNPCUI(pDungeonListWnd);
}

void TeleportCallback(CNPC* _this)
{
	CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(_this->GetTeleportDest()));
}

void TeleportBtnCallback(CButton* _this)
{
	CLevelMgr::GetInst()->ChangeLevel(CLevelMgr::GetInst()->FindLevel(_this->GetName()));
}