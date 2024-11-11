#include "pch.h"
#include "CLargo_Death.h"
#include "CObj.h"
#include "CRigidBody.h"
#include "CLargo.h"
#include "CAlbumPlayer.h"
#include "CLevelMgr.h"
#include "CSoundMgr.h"
#include "CSound.h"
#include "CS03.h"

CLargo_Death::CLargo_Death()
	: CState(L"Largo_Death")
{
}

CLargo_Death::~CLargo_Death()
{
}

void CLargo_Death::Enter()
{
	GetOwnerObj()->SetState((int)LargoState::Death);
	GetOwnerObj()->GetRigidBody()->SetSpeed(Vec2D(0, 0));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 5);
	CSoundMgr::GetInst()->GetSound(L"largo_4th_vo_die_0" + std::to_wstring(dis(gen))
		, L"\\sound\\sounds_mon_duskyisland_vo\\largo_4th_vo_die_0" + std::to_wstring(dis(gen)) + L".ogg")->Play();
}

void CLargo_Death::FinalTick()
{
	vector<CAlbumPlayer*>& vec0 = ((CLargo*)GetOwnerObj())->GetAnimationVec(LargoState::Death);
	if (vec0[0]->GetCurSceneNum() == vec0[0]->GetFinal())
	{
		CS03* pLevel = (CS03*)CLevelMgr::GetInst()->GetCurrentLevel();
		pLevel->DeleteObject(GetOwnerObj()->GetLayerType(), GetOwnerObj()->GetID());
		pLevel->SetClear(true);
	}
}

void CLargo_Death::Exit()
{
}
