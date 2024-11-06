#pragma once

#include <chrono>

class CTimeMgr
{
	SINGLETON(CTimeMgr);

private:
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_PrevCount;
	LARGE_INTEGER	m_CurCount;

	float			m_DT;
	UINT			m_FPS;
	float			m_Time;


	std::chrono::steady_clock::time_point m_CheckTimer;
	std::chrono::nanoseconds m_CheckDuration;

public:
	void Init();
	void Tick();
	float GetDeltaTime() { return m_DT; }

	void TimeCheckStart();
	LONGLONG TimeCheck();
};

