#include "pch.h"
#include "CTimeMgr.h"

CTimeMgr::CTimeMgr()
	: m_CurCount{}
	, m_DT(0)
	, m_Frequency{}
	, m_PrevCount{}
{
	
}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Init()
{
	QueryPerformanceFrequency(&m_Frequency);	// 초당 카운팅 횟수
	QueryPerformanceCounter(&m_PrevCount);
}

void CTimeMgr::Tick()
{
	QueryPerformanceCounter(&m_CurCount);
	m_DT = (float)(m_CurCount.QuadPart - m_PrevCount.QuadPart) / (float)(m_Frequency.QuadPart);
	m_PrevCount = m_CurCount;
}
