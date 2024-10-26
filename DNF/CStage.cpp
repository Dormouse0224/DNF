#include "pch.h"
#include "CStage.h"
#include "CBackground.h"



CStage::CStage(wstring _name)
	: CLevel(_name)
	, m_StageInfo(nullptr)
	, m_BGM(nullptr)
{
}

CStage::~CStage()
{
}

void CStage::Begin()
{

}

void CStage::Tick()
{
	CLevel::Tick();

}

void CStage::FinalTick()
{
	CLevel::FinalTick();
}

void CStage::Render()
{
	CLevel::Render();
}

void CStage::End()
{
	ClearAll();
}

