#include "pch.h"
#include "CPlayer.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CFSM.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CDbgRender.h"
#include "CTimeMgr.h"
#include "CTextureMgr.h"
#include "CDummy.h"
#include "CCameraMgr.h"
#include "CLevelMgr.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CKeyMgr.h"
#include "CUI.h"
#include "CSticker.h"
#include "CWindow.h"
#include "CButton.h"
#include "CItemIcon.h"

#include "CPlayer_Idle.h"
#include "CPlayer_Walk.h"
#include "CPlayer_Run.h"
#include "CPlayer_Jump.h"
#include "CPlayer_Attack.h"
#include "CPlayer_Death.h"


CPlayer::CPlayer()
	: CObj(L"Player")
	, m_YogiGauge(0.f)
	, m_Avatar{}
	, m_AttackState(nullptr)
	, m_CurAvatarCode{ "0000", "0000" , "0000" , "0000" , "0000" , "0000" }
	, m_PrevAvatarCode{ "0000", "0000" , "0000" , "0000" , "0000" , "0000" }
	, m_HUD(nullptr)
	, m_HPTex(nullptr)
	, m_MPTex(nullptr)
	, m_bInventoryOn(false)
	, m_InventoryID(0)
	, m_InvenSlot(nullptr)
	, m_EquipVec((int)EquipParts::END, nullptr)
	, m_AvatarEquipVec((int)AvatarParts::END, nullptr)
{
	m_MaxHP = 1000;
	m_CurHP = 1000;
	m_MaxMP = 1000;
	m_CurMP = 1000;

	m_HPRegen = m_MaxHP * 0.01f;
	m_MPRegen = m_MaxMP * 0.1f;

	// 인벤토리 목록 비우기 (초기화)
	for (int i = 0; i < (int)ItemType::END; ++i)
	{
		m_InvenVec[i].resize(64, nullptr);
	}

	// 스킬 리소스 미리 로드
	CTextureMgr::PreloadFromFile(L"PlayerResources.txt");
	CTextureMgr::PreloadAvatar(L"AvatarResources.txt");

	// 상태별 모션 애니메이션
	for (int i = 0; i < (int)PlayerState::END; ++i) {
		m_Avatar[i] = std::vector<CAlbumPlayer*>(25, nullptr); // 각 벡터의 크기를 25로 설정하고 nullptr로 초기화
	}
	SetLayerType(LayerType::Object);
	SetScale(Vec2D(40, 95));
	AddAnimation(PlayerState::Idle, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Idle.animation"));
	CAlbumPlayer::CreateAvatar(L"archer_skin_00_Idle", m_CurAvatarCode, m_Avatar[(int)PlayerState::Idle]);
	AddAnimation(PlayerState::Walk, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Walk"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Walk.animation"));
	CAlbumPlayer::CreateAvatar(L"archer_skin_00_Walk", m_CurAvatarCode, m_Avatar[(int)PlayerState::Walk]);
	AddAnimation(PlayerState::Run, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Run"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Run.animation"));
	CAlbumPlayer::CreateAvatar(L"archer_skin_00_Run", m_CurAvatarCode, m_Avatar[(int)PlayerState::Run]);
	AddAnimation(PlayerState::Jump, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Jump"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Jump.animation"));
	CAlbumPlayer::CreateAvatar(L"archer_skin_00_Jump", m_CurAvatarCode, m_Avatar[(int)PlayerState::Jump]);
	AddAnimation(PlayerState::Death, CAlbumPlayer::CreatePlayerFromFile(L"archer_skin_00_Down"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\archer_skin_00_Down.animation"));
	CAlbumPlayer::CreateAvatar(L"archer_skin_00_Down", m_CurAvatarCode, m_Avatar[(int)PlayerState::Death]);

	// 상태 등록
	CFSM* pFSM = new CFSM(L"Player_FSM");
	pFSM->AddState((int)PlayerState::Idle, new CPlayer_Idle(L"Player_Idle"));
	pFSM->AddState((int)PlayerState::Walk, new CPlayer_Walk(L"Player_Walk"));
	pFSM->AddState((int)PlayerState::Run, new CPlayer_Run(L"Player_Run"));
	pFSM->AddState((int)PlayerState::Jump, new CPlayer_Jump(L"Player_Jump"));
	m_AttackState = new CPlayer_Attack(L"Player_Attack");
	pFSM->AddState((int)PlayerState::Attack, m_AttackState);
	pFSM->AddState((int)PlayerState::Death, new CPlayer_Death(L"Player_Death"));
	AddComponent(pFSM);

	// 강체 등록
	SetRigidBody(new CRigidBody(L"Player_RB"));

	// 충돌체 등록
	CCollider* pCol = new CCollider(L"Player_Col");
	RegisterBodyCollider(pCol);
	AddComponent(pCol);

	// 플레이어 HUD 등록
	m_HUD = new CDummy(L"hud");
	m_HUD->SetLocation(CCameraMgr::GetInst()->GetCameraPos() + Vec2D(347, 515));
	m_HUD->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_skillwindow"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_skillwindow.animation"));
	m_HUD->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_hpmpvessel"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_hpmpvessel.animation", Vec2D(25, 20)));
	m_HPTex = CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_hp"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_hp.animation", Vec2D(25, 20));
	m_HUD->AddComponent(m_HPTex);
	m_HUD->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_hpmpvessel"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_hpmpvessel.animation", Vec2D(319, 20)));
	m_MPTex = CAlbumPlayer::CreatePlayerFromFile(L"ui_hud_mp"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_hud_mp.animation", Vec2D(319, 20));
	m_HUD->AddComponent(m_MPTex);
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_HUD, LayerType::Near);



	ItemInfo* test = new ItemInfo();
	test->Name = "5400";
	test->Item_type = ItemType::Avatar;
	test->Item_secondtype = (int)AvatarParts::LBow;
	test->Texture_ani = L"icontest";
	m_InvenVec[(int)ItemType::Avatar][0] = test;


	ItemInfo* test1 = new ItemInfo();
	test1->Name = "5000";
	test1->Item_type = ItemType::Avatar;
	test1->Item_secondtype = (int)AvatarParts::LBow;
	test1->Texture_ani = L"icontest1";
	m_InvenVec[(int)ItemType::Avatar][1] = test1;
}

CPlayer::~CPlayer()
{
	for (int i = 0; i < (int)PlayerState::END; ++i)
	{
		for (int j = 0; j < 25; ++j)
		{
			delete m_Avatar[i][j];
		}
		for (int j = 0; j < m_Animation[i].size(); ++j)
		{
			delete m_Animation[i][j];
		}
	}

	for (int i = 0; i < m_EquipVec.size(); ++i)
	{
		delete m_EquipVec[i];
	}
	for (int i = 0; i < m_AvatarEquipVec.size(); ++i)
	{
		delete m_AvatarEquipVec[i];
	}
	for (int i = 0; i < (int)ItemType::END; ++i)
	{
		for (int j = 0; j < m_InvenVec[i].size(); ++j)
		{
			delete m_InvenVec[i][j];
		}
	}
}

void CPlayer::BeginOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CPlayer::Overlap(CCollider* _Self, CCollider* _Other)
{
}

void CPlayer::EndOverlap(CCollider* _Self, CCollider* _Other)
{
}

void CPlayer::GiveDamage(int _dmg)
{
	if (m_Immune)
		return;
	CObj::GiveDamage(_dmg);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 4);
	CSoundMgr::GetInst()->GetSound(L"ar_dmg_0" + std::to_wstring(dis(gen))
		, L"\\sound\\sounds_char_archer\\ar_dmg_0" + std::to_wstring(dis(gen)) + L".ogg")->Play();
}

void CPlayer::Begin()
{
}

void CPlayer::Tick()
{
	// 장착 아바타 코드 갱신
	for (int i = 0; i < (int)AvatarParts::END; ++i)
	{
		if (m_AvatarEquipVec[i])
		{
			m_CurAvatarCode[i] = m_AvatarEquipVec[i]->Name;
		}
		else
		{
			m_CurAvatarCode[i] = "0000";
		}
	}

	// hud 위치 갱신
	m_HUD->SetLocation(CCameraMgr::GetInst()->GetCameraPos() + Vec2D(347, 525));

	// hp, mp 재생
	m_CurHP = min(m_HPRegen * CTimeMgr::GetInst()->GetDeltaTime() + m_CurHP, m_MaxHP);
	m_CurMP = min(m_MPRegen * CTimeMgr::GetInst()->GetDeltaTime() + m_CurMP, m_MaxMP);

	// hp, mp 퍼센티지 갱신
	m_HPTex->SetRenderPercent(m_CurHP / m_MaxHP);
	m_MPTex->SetRenderPercent(m_CurMP / m_MaxMP);

	// 바닥 위치 갱신
	Vec2D gp(GetLocation().x + (GetScale().x / 2.f), GetLocation().y + GetScale().y);
	if (GetRigidBody() != nullptr)
	{
		RenewGroundPos(gp - Vec2D(0.f, GetRigidBody()->GetAirborneHeight()));
	}
	else
	{
		RenewGroundPos(gp);
	}

	// 그라운드 포지션 디버그 렌더링
	CDbgRender::GetInst()->AddDbgRender(DbgRenderShape::Circle, GetGroundPos() - Vec2D(5, 5)
		, Vec2D(10, 10), 0, Color(255, 0, 0, 255));

	// 바라보는 방향 갱신
	if (GetRigidBody()->GetSpeed().x < 0)
	{
		m_bLookLeft = true;
	}
	else if (GetRigidBody()->GetSpeed().x > 0)
	{
		m_bLookLeft = false;
	}
	
	// 요기 게이지 감소
	if (m_YogiGauge > 0)
	{
		m_YogiGauge -= CTimeMgr::GetInst()->GetDeltaTime() * 10;
		if (m_YogiGauge < 0)
			m_YogiGauge = 0;
	}

	int state = GetState();
	for (int i = 0; i < m_Animation[state].size(); ++i)
	{
		m_Animation[state][i]->FinalTick();
	}
	for (int i = 0; i < 25; ++i)
	{
		if (m_Avatar[state][i])
			m_Avatar[state][i]->FinalTick();
	}

	if (CheckAvatarCodeChange())
	{
		CAlbumPlayer::CreateAvatar(L"archer_skin_00_Idle", m_CurAvatarCode, m_Avatar[(int)PlayerState::Idle]);
		CAlbumPlayer::CreateAvatar(L"archer_skin_00_Walk", m_CurAvatarCode, m_Avatar[(int)PlayerState::Walk]);
		CAlbumPlayer::CreateAvatar(L"archer_skin_00_Run", m_CurAvatarCode, m_Avatar[(int)PlayerState::Run]);
		CAlbumPlayer::CreateAvatar(L"archer_skin_00_Jump", m_CurAvatarCode, m_Avatar[(int)PlayerState::Jump]);
	}

	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::I) == Key_state::TAP && m_bInventoryOn == false)
	{
		m_bInventoryOn = true;
		OpenInventory();
	}
	else if (CKeyMgr::GetInst()->GetKeyState(Keyboard::I) == Key_state::TAP && m_bInventoryOn == true)
	{
		m_bInventoryOn = false;
		if (m_InventoryID != 0)
		{
			CLevelMgr::GetInst()->GetCurrentLevel()->DeleteObject(LayerType::UI, m_InventoryID);
			m_InventoryID = 0;
		}
	}
}

void CPlayer::Render()
{
	if (GetState() == (int)PlayerState::Attack)
	{
		m_AttackState->Render();
	}
	else
	{
		int state = GetState();
		for (int i = 0; i < m_Animation[state].size(); ++i)
		{
			m_Animation[state][i]->Render(this, false, m_bLookLeft);
		}
		for (int i = 0; i < 25; ++i)
		{
			if (m_Avatar[state][i])
				m_Avatar[state][i]->Render(this, false, m_bLookLeft);
		}
	}

	// 인벤토리창이 열려있는경우 인벤토리 내용물 렌더링

}

void CPlayer::AddAnimation(PlayerState _state, CAlbumPlayer* _animation)
{
	m_Animation[(int)_state].push_back(_animation);
}

void CPlayer::StateAnimationInit(PlayerState _state)
{
	for (int i = 0; i < m_Animation[(int)_state].size(); ++i)
	{
		if (m_Animation[(int)_state][i])
			m_Animation[(int)_state][i]->SetCurrentScene(0);
	}
	for (int i = 0; i < m_Avatar[(int)_state].size(); ++i)
	{
		if (m_Avatar[(int)_state][i])
			m_Avatar[(int)_state][i]->SetCurrentScene(0);
	}
}

bool CPlayer::CheckAvatarCodeChange()
{
	for (int i = 0; i < 6; ++i)
	{
		if (m_CurAvatarCode[i] != m_PrevAvatarCode[i])
		{
			m_PrevAvatarCode[i] = m_CurAvatarCode[i];
			return true;
		}
	}
	return false;
}

void CPlayer::OpenInventory()
{
	// 인벤토리창 틀 추가
	CWindow* pInventoryWindow = new CWindow(L"InventoryWindow");
	m_InventoryID = pInventoryWindow->GetID();
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pInventoryWindow, LayerType::UI);
	pInventoryWindow->SetWindowColor(Color(255, 0, 0, 0));
	pInventoryWindow->SetUIPos(Vec2D(700, 100));
	pInventoryWindow->SetScale(Vec2D(254, 475));
	pInventoryWindow->SetMovalbe(true);

	// 인벤토리탭, 아바타탭 버튼 추가
	CButton* pInventoryTab = new CButton(L"InventoryBtn");
	pInventoryTab->SetUIPos(Vec2D(3, 3));
	pInventoryTab->SetScale(Vec2D(82, 22));
	pInventoryTab->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_inventory_idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_inventory_idle.animation"), BtnState::IDLE);
	pInventoryTab->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_inventory_cursoron"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_inventory_cursoron.animation"), BtnState::CURSOR_ON);
	pInventoryTab->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_inventory_idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_inventory_idle.animation"), BtnState::PRESSED);
	pInventoryTab->SetDelegate1(this, (DELEGATE_1)&CPlayer::TabBtnDelegate, (DWORD_PTR)pInventoryTab);

	CButton* pAvatarTab = new CButton(L"AvatarBtn");
	pAvatarTab->SetUIPos(Vec2D(86, 3));
	pAvatarTab->SetScale(Vec2D(82, 22));
	pAvatarTab->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_avatar_idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_avatar_idle.animation"), BtnState::IDLE);
	pAvatarTab->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_avatar_cursoron"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_avatar_cursoron.animation"), BtnState::CURSOR_ON);
	pAvatarTab->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_avatar_idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_avatar_idle.animation"), BtnState::PRESSED);
	pAvatarTab->SetDelegate1(this, (DELEGATE_1)&CPlayer::TabBtnDelegate, (DWORD_PTR)pAvatarTab);

	pInventoryWindow->AddChild(pAvatarTab, L"AvatarBtn");
	pInventoryWindow->AddChild(pInventoryTab, L"InventoryBtn");


	// 아바타탭 요소 추가
	CWindow* pAvatarBackground = new CWindow(L"AvatarBackground");
	pAvatarBackground->SetWindowColor(Color(255, 50, 50, 50));
	pAvatarBackground->SetUIPos(Vec2D(-83, 22));
	pAvatarBackground->SetScale(Vec2D(248, 447));

	CWindow* pAvatarBackgroundEquip = new CWindow(L"AvatarBackgroundEquip");
	pAvatarBackgroundEquip->SetUIPos(Vec2D(-81, 22));
	pAvatarBackgroundEquip->SetScale(Vec2D(244, 154));
	pAvatarBackgroundEquip->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_avatar_background"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_avatar_background.animation"));
	pAvatarBackgroundEquip->SetUIItemType(ItemType::Avatar);
	pAvatarBackgroundEquip->SetUIInventoryType(InventoryType::Equip);

	CWindow* pPlayerStatue_Avt = new CWindow(L"PlayerStatue_Avt");
	pPlayerStatue_Avt->SetUIPos(Vec2D(-54, 64));
	pPlayerStatue_Avt->SetScale(Vec2D(40, 95));
	pPlayerStatue_Avt->SetRenderDelegate1(this, (DELEGATE_1)&CPlayer::RenderPlayerStatue, (DWORD_PTR)pPlayerStatue_Avt);

	CButton* pAvatarBtn = new CButton(L"AvatarBtn");
	pAvatarBtn->SetUIPos(Vec2D(-83, 205));
	pAvatarBtn->SetScale(Vec2D(82, 22));
	pAvatarBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_avatar_idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_avatar_idle.animation"), BtnState::IDLE);
	pAvatarBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_avatar_cursoron"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_avatar_cursoron.animation"), BtnState::CURSOR_ON);
	pAvatarBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_avatar_idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_avatar_idle.animation"), BtnState::PRESSED);
	pAvatarBtn->SetDelegate1(this, (DELEGATE_1)&CPlayer::TabBtnDelegate, (DWORD_PTR)pAvatarBtn);

	pAvatarTab->AddChild(pAvatarBackground, L"AvatarBackground");
	pAvatarTab->AddChild(pAvatarBackgroundEquip, L"AvatarBackgroundEquip");
	pAvatarTab->AddChild(pPlayerStatue_Avt, L"PlayerStatue_Avt");
	pAvatarTab->AddChild(pAvatarBtn, L"AvatarBtn");

	// 인벤토리탭 요소 추가
	CWindow* pInventoryBackground = new CWindow(L"InventoryBackground");
	pInventoryBackground->SetUIPos(Vec2D(0, 22));
	pInventoryBackground->SetScale(Vec2D(248, 447));
	pInventoryBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_inventory_background"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_inventory_background.animation"));

	CWindow* pInventoryBackgroundEquip = new CWindow(L"InventoryBackgroundEquip");
	pInventoryBackgroundEquip->SetUIPos(Vec2D(5, 27));
	pInventoryBackgroundEquip->SetScale(Vec2D(238, 96));
	pInventoryBackgroundEquip->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_inventory_background_equip"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_inventory_background_equip.animation"));
	pInventoryBackgroundEquip->SetUIItemType(ItemType::Equip);
	pInventoryBackgroundEquip->SetUIInventoryType(InventoryType::Equip);

	CWindow* pPlayerStatue = new CWindow(L"PlayerStatue");
	pPlayerStatue->SetUIPos(Vec2D(104, 64));
	pPlayerStatue->SetScale(Vec2D(40, 95));
	pPlayerStatue->SetRenderDelegate1(this, (DELEGATE_1)&CPlayer::RenderPlayerStatue, (DWORD_PTR)pPlayerStatue);

	CButton* pEquipBtn = new CButton(L"EquipBtn");
	pEquipBtn->SetUIPos(Vec2D(0, 205));
	pEquipBtn->SetScale(Vec2D(82, 22));
	pEquipBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_equip_idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_equip_idle.animation"), BtnState::IDLE);
	pEquipBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_equip_cursoron"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_equip_cursoron.animation"), BtnState::CURSOR_ON);
	pEquipBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_equip_idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_equip_idle.animation"), BtnState::PRESSED);
	pEquipBtn->SetDelegate1(this, (DELEGATE_1)&CPlayer::TabBtnDelegate, (DWORD_PTR)pEquipBtn);

	CButton* pConsumableBtn = new CButton(L"ConsumableBtn");
	pConsumableBtn->SetUIPos(Vec2D(83, 205));
	pConsumableBtn->SetScale(Vec2D(82, 22));
	pConsumableBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_consumable_idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_consumable_idle.animation"), BtnState::IDLE);
	pConsumableBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_consumable_cursoron"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_consumable_cursoron.animation"), BtnState::CURSOR_ON);
	pConsumableBtn->AddComponent_Btn(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_btn_consumable_idle"
		, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_btn_consumable_idle.animation"), BtnState::PRESSED);
	pConsumableBtn->SetDelegate1(this, (DELEGATE_1)&CPlayer::TabBtnDelegate, (DWORD_PTR)pConsumableBtn);

	pInventoryTab->AddChild(pInventoryBackground, L"InventoryBackground");
	pInventoryTab->AddChild(pInventoryBackgroundEquip, L"InventoryBackgroundEquip");
	pInventoryTab->AddChild(pPlayerStatue, L"PlayerStatue");
	pInventoryTab->AddChild(pConsumableBtn, L"ConsumableBtn");
	pInventoryTab->AddChild(pEquipBtn, L"EquipBtn");


	// 장비탭, 소비탭, 아바타탭 인벤토리 슬롯
	CWindow* pEquipSlot = new CWindow(L"EquipSlot");
	pEquipSlot->SetWindowColor(Color(255, 50, 50, 50));
	pEquipSlot->SetUIPos(Vec2D(4, 22));
	pEquipSlot->SetScale(Vec2D(240, 240));
	pEquipSlot->SetUIItemType(ItemType::Equip);
	pEquipSlot->SetUIInventoryType(InventoryType::Slot);
	for (int row = 0; row < 8; ++row)
	{
		for (int col = 0; col < 8; ++col)
		{
			pEquipSlot->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_inventory_slot"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_inventory_slot.animation", Vec2D(row, col) * 30));
		}
	}

	CWindow* pConsumableSlot = new CWindow(L"ConsumableSlot");
	pConsumableSlot->SetWindowColor(Color(255, 50, 50, 50));
	pConsumableSlot->SetUIPos(Vec2D(-79, 22));
	pConsumableSlot->SetScale(Vec2D(240, 240));
	pConsumableSlot->SetUIItemType(ItemType::Consumable);
	pConsumableSlot->SetUIInventoryType(InventoryType::Slot);
	for (int row = 0; row < 7; ++row)
	{
		for (int col = 0; col < 7; ++col)
		{
			pConsumableSlot->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_inventory_slot"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_inventory_slot.animation", Vec2D(row, col) * 30));
		}
	}


	CWindow* pAvatarSlot = new CWindow(L"AvatarSlot");
	pAvatarSlot->SetWindowColor(Color(255, 50, 50, 50));
	pAvatarSlot->SetUIPos(Vec2D(4, 22));
	pAvatarSlot->SetScale(Vec2D(240, 240));
	pAvatarSlot->SetUIItemType(ItemType::Avatar);
	pAvatarSlot->SetUIInventoryType(InventoryType::Slot);
	for (int row = 0; row < 6; ++row)
	{
		for (int col = 0; col < 6; ++col)
		{
			pAvatarSlot->AddComponent(CAlbumPlayer::CreatePlayerFromFile(L"ui_window_inventory_slot"
				, CEngine::GetInst()->GetResourcePathW() + L"\\animation\\ui_window_inventory_slot.animation", Vec2D(row, col) * 30));
		}
	}

	pConsumableBtn->AddChild(pConsumableSlot, L"ConsumableSlot");
	pEquipBtn->AddChild(pEquipSlot, L"EquipSlot");

	pAvatarBtn->AddChild(pAvatarSlot, L"AvatarSlot");

	// 아이템 추가
	for (int j = 0; j < (int)ItemType::END; ++j)
	{
		for (int i = 0; i < m_InvenVec[j].size(); ++i)
		{
			if (m_InvenVec[j][i])
			{
				CItemIcon* pItem = new CItemIcon(wstring(m_InvenVec[j][i]->Name.begin(), m_InvenVec[j][i]->Name.end()), i);
				pItem->SetUIPos(Vec2D((i % 8) * 30, (i / 8) * 30));
				pItem->SetItemInfo(m_InvenVec[j][i]);
				pAvatarSlot->AddChild(pItem, pItem->GetName());
			}
		}
	}

	for (int i = 0; i < m_EquipVec.size(); ++i)
	{
		if (m_EquipVec[i])
		{
			CItemIcon* pItem = new CItemIcon(wstring(m_EquipVec[i]->Name.begin(), m_EquipVec[i]->Name.end()), i);
			pItem->SetEquipIconPos(i);
			pItem->SetItemInfo(m_EquipVec[i]);
			pInventoryBackgroundEquip->AddChild(pItem, pItem->GetName());
		}
	}

	for (int i = 0; i < m_AvatarEquipVec.size(); ++i)
	{
		if (m_AvatarEquipVec[i])
		{
			CItemIcon* pItem = new CItemIcon(wstring(m_AvatarEquipVec[i]->Name.begin(), m_AvatarEquipVec[i]->Name.end()), i);
			pItem->SetAvatarIconPos(i);
			pItem->SetItemInfo(m_AvatarEquipVec[i]);
			pAvatarBackgroundEquip->AddChild(pItem, pItem->GetName());
		}
	}
}

void CPlayer::TabBtnDelegate(DWORD_PTR _param)
{
	vector<CUI*>& ChVec = ((CUI*)_param)->GetParent()->GetChildVec();
	vector<CUI*>::iterator iter = find(ChVec.begin(), ChVec.end(), ((CUI*)_param));
	ChVec.erase(iter);
	ChVec.push_back(((CUI*)_param));
}

void CPlayer::RenderPlayerStatue(DWORD_PTR _param)
{
	for (int i = 0; i < m_Animation[(int)PlayerState::Idle].size(); ++i)
	{
		m_Animation[(int)PlayerState::Idle][i]->Render((CObj*)_param);
	}
	for (int i = 0; i < 25; ++i)
	{
		if (m_Avatar[(int)PlayerState::Idle][i])
			m_Avatar[(int)PlayerState::Idle][i]->Render((CObj*)_param);
	}
}


