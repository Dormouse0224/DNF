#pragma once
class CCameraMgr
{
public:
	SINGLETON(CCameraMgr);


private:
	Vec2D m_CameraPos;
	float m_CameraSpeed;

	float m_Alpha;

public:
	void Init();
	void Tick();
	Vec2D GetCameraPos() { return m_CameraPos; }
	void InitCameraPos() { m_CameraPos = Vec2D(0, 0); }
};

