#pragma once
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

public:
	void Init();
	void Tick();
	float GetDeltaTime() { return m_DT; }
};

