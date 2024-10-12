#pragma once
class CCameraMgr
{
public:
	SINGLETON(CCameraMgr);


private:
	Vec2D m_CameraPos;
	float m_CameraSpeed;

public:
	void Init();
	void Tick();
	Vec2D GetCameraPos() { return m_CameraPos; }
};

