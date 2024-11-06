#pragma once



class CCameraMgr
{
public:
	SINGLETON(CCameraMgr);


private:
	Vec2D m_CameraPos;
	float m_CameraSpeed;
	CameraEffect m_Effect;
	float m_EffectTimer;
	bool m_IsEffectFin;
	int m_Alpha;

public:
	void Init();
	void Tick();
	Vec2D GetCameraPos() { return m_CameraPos; }
	void InitCameraPos() { m_CameraPos = Vec2D(0, 0); }

	void SetEffect(CameraEffect _eff) { m_Effect = _eff; }
	bool IsEffectFin();

	void Effect();
};

