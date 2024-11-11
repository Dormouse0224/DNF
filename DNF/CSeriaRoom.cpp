#include "pch.h"
#include "CSeriaRoom.h"
#include "CBackground.h"
#include "CAlbumPlayer.h"
#include "CEngine.h"
#include "CPlayer.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CCameraMgr.h"

CSeriaRoom::CSeriaRoom()
	: CStage(L"SeriaRoom")
{
	m_StageInfo = new StageInfo();
	m_StageInfo->StageSize = Vec2D(1119, 600);
	m_StageInfo->StageName = GetName();
	m_StageInfo->BGMPath = L"seria_gate.ogg";
	m_UpperBound = 260;
}

CSeriaRoom::~CSeriaRoom()
{
}

void CSeriaRoom::Begin()
{
	SetCameraFollowPlayer(true);
	CCameraMgr::GetInst()->SetEffect(CameraEffect::FadeIn);

	// 배경 객체 생성 후 애니메이션 입력
	CBackground* pBackground = new CBackground(GetName() + L"_Background_0");
	pBackground->SetScale(m_StageInfo->StageSize);
	AddObject(pBackground, LayerType::Background);
	pBackground->AddComponent(CAlbumPlayer::CreatePlayerFromFile(pBackground->GetName(), CEngine::GetInst()->GetResourcePathW()
		+ L"\\animation\\seria_room_background.animation"));


	// 플레이어 추가
	CPlayer* pPlayer = new CPlayer();
	AddObject(pPlayer, LayerType::Object);
	SetPlayer(pPlayer);
	pPlayer->SetLocation(Vec2D(600, 450));

	// BGM이 있으면 재생
	if (!m_StageInfo->BGMPath.empty())
	{
		m_BGM = CSoundMgr::GetInst()->GetSound(m_StageInfo->BGMPath, L"\\music\\" + m_StageInfo->BGMPath);
		m_BGM->PlayToBGM(true);
	}
}

void CSeriaRoom::Tick()
{
	CStage::Tick();
}

void CSeriaRoom::FinalTick()
{
	CStage::FinalTick();
}

void CSeriaRoom::Render()
{
	CStage::Render();
}

void CSeriaRoom::End()
{
	CStage::End();
}
