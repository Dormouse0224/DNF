#pragma once
class CTimeMgr
{
	SINGLETON(CTimeMgr);

private:
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_PrevCount;
	LARGE_INTEGER	m_CurCount;

	float			m_DT;

public:
	void Init();
	void Tick();
	float GetDeltaTime() { return m_DT; }
};

