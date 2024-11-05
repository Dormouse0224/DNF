#include "pch.h"
#include "CS03.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CPlayer.h"
#include "CTimeMgr.h"
#include "CRigidBody.h"
#include "CCameraMgr.h"
#include "CLevelMgr.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CDummy.h"
#include "CLargo.h"
#include "CSea.h"

CS03::CS03()
	: CStage(L"S03")
	, m_Largo(nullptr)
	, m_AoEVec{}
	, m_SeaGap(150)
{
	m_StageInfo = new StageInfo();
	m_StageInfo->StageSize = Vec2D(1568, 960);
	m_StageInfo->StageName = GetName();
}

CS03::~CS03()
{
	for (int i = 0; i < m_AoEVec.size(); ++i)
	{
		if (m_AoEVec[i] != nullptr)
		{
			delete m_AoEVec[i];
		}
	}
	m_AoEVec.clear();

}

void CS03::Begin()
{
	CCameraMgr::GetInst()->InitCameraPos();
	CCameraMgr::GetInst()->SetEffect(CameraEffect::FadeIn);

	// 배경 객체 생성 후 애니메이션 입력
	CBackground* pBackground = new CBackground(GetName() + L"_Background_0");
	pBackground->SetScale(m_StageInfo->StageSize);
	AddObject(pBackground, LayerType::Background);
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_brokenwatchtower_background.animation"));

	CDummy* pDummy0 = new CDummy(L"Dummy0");
	pDummy0->SetScale(Vec2D(431, 200));
	pDummy0->SetLocation(Vec2D(0, 360));
	AddObject(pDummy0, LayerType::Object);
	pDummy0->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pDummy0->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_brokenwatchtower_obj_0.animation"));

	CDummy* pDummy1 = new CDummy(L"Dummy1");
	pDummy1->SetScale(Vec2D(590, 300));
	pDummy1->SetLocation(Vec2D(1050, 620));
	AddObject(pDummy1, LayerType::Object);
	pDummy1->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pDummy1->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\map_brokenwatchtower_obj_1.animation"));

	// 요기 게이지 추가
	m_PlayerYogiGauge = new CDummy(L"PlayerYogi");
	AddObject(m_PlayerYogiGauge, LayerType::Near);
	m_PlayerYogiGauge->AddComponent(CAlbumPlayer::CreatePlayerFromFile(m_PlayerYogiGauge->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\boss_unshackled_pattern_YogiGauge_Player_Background.animation"));
	m_PlayerYogi = CAlbumPlayer::CreatePlayerFromFile(m_PlayerYogiGauge->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\boss_unshackled_pattern_YogiGauge_Player_Charge.animation");
	m_PlayerYogiGauge->AddComponent(m_PlayerYogi);
	m_PlayerYogi->SetRenderPercent(0.f);


	// 보스 추가
	m_Largo = new CLargo();
	AddObject(m_Largo, LayerType::Object);
	m_Largo->SetLocation(Vec2D(740, 500));

	// 바다 추가
	m_Sea[0] = new CSea(L"TopSea");
	AddObject(m_Sea[0], LayerType::Far);
	m_Sea[0]->SetScale(Vec2D(1568, 1));
	m_Sea[0]->SetLocation(Vec2D(0, 300));
	m_Sea[0]->AddComponent(CAlbumPlayer::CreatePlayerFromFile(m_Sea[0]->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\boss_unshackled_effect_wave_top.animation", Vec2D(392, 0)));
	m_Sea[0]->AddComponent(CAlbumPlayer::CreatePlayerFromFile(m_Sea[0]->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\boss_unshackled_effect_wave_top.animation", Vec2D(1176, 0)));

	m_Sea[1] = new CSea(L"BottomSea");
	AddObject(m_Sea[1], LayerType::Far);
	m_Sea[1]->SetScale(Vec2D(1568, 1));
	m_Sea[1]->SetLocation(Vec2D(0, 700));
	m_Sea[1]->AddComponent(CAlbumPlayer::CreatePlayerFromFile(m_Sea[1]->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\boss_unshackled_effect_wave_bottom.animation", Vec2D(392, 0)));
	m_Sea[1]->AddComponent(CAlbumPlayer::CreatePlayerFromFile(m_Sea[1]->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\boss_unshackled_effect_wave_bottom.animation", Vec2D(1176, 0)));



	// 플레이어 추가
	CPlayer* pPlayer = new CPlayer();
	AddObject(pPlayer, LayerType::Object);
	SetPlayer(pPlayer);
	pPlayer->SetLocation(Vec2D(1000, 500));


	SetCameraFollowPlayer(true);
}

void CS03::Tick()
{
	for (vector<CAoE*>::iterator iter = m_AoEVec.begin(); iter != m_AoEVec.end();)
	{
		(*iter)->Tick();
		if ((*iter)->IsEffectFin())
		{
			delete* iter;
			iter = m_AoEVec.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	// 바다 위치 갱신
	m_Sea[0]->SetLocation(Vec2D(0.f, m_Largo->GetGroundPos().y - m_SeaGap));
	m_Sea[1]->SetLocation(Vec2D(0.f, m_Largo->GetGroundPos().y + m_SeaGap));

	// 바다에 플레이가 있으면 플레이어 요기 게이지 증가
	if ((GetPlayer()->GetGroundPos().y > m_Sea[1]->GetGroundPos().y || GetPlayer()->GetGroundPos().y < m_Sea[0]->GetGroundPos().y) && !(GetPlayer()->GetRigidBody()->GetAirborne()))
		GetPlayer()->AddYogiGauge(CTimeMgr::GetInst()->GetDeltaTime() * 40.f);
	m_PlayerYogi->SetRenderPercent(min(GetPlayer()->GetYogiGauge() / 100.f, 1.f));

	// 요기 게이지 포지션 갱신
	m_PlayerYogiGauge->SetLocation(GetPlayer()->GetLocation() + Vec2D(70, -20));


	CLevel::Tick();
}

void CS03::FinalTick()
{

	CLevel::FinalTick();
}

void CS03::Render()
{
	for (int i = 0; i < (int)LayerType::Object; ++i)
	{
		vector<CObj*> vec = GetObjLayer((LayerType)i);
		for (int j = 0; j < vec.size(); ++j)
		{
			if (vec[j]->GetDead() == false)
			{
				vec[j]->Render();
			}
		}
	}

	// AoE 를 Far 와 Object 레이어 사이에 렌더링
	for (CAoE* pAoE : m_AoEVec)
	{
		pAoE->Render();
	}

	for (int i = (int)LayerType::Object; i < (int)LayerType::END; ++i)
	{
		vector<CObj*> vec = GetObjLayer((LayerType)i);
		for (int j = 0; j < vec.size(); ++j)
		{
			if (vec[j]->GetDead() == false)
			{
				vec[j]->Render();
			}
		}
	}
}

void CS03::End()
{

	ClearAll();
}

void CS03::AddAoE(AoEType _type, Vec2D _pos, int _param1, int _param2, float _castTime, bool _isVert)
{
	CAoE* pAoE = new CAoE(L"AoE");
	pAoE->SetInfo(_type, _pos, _param1, _param2, _castTime, _isVert);
	m_AoEVec.push_back(pAoE);
}
