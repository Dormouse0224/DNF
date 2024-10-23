#pragma once
class CDbgRender
{
	SINGLETON(CDbgRender);

private:
	list<DbgRenderInfo> m_DbgRenderList;
	bool m_Show;

public:
	void AddDbgRender(DbgRenderShape _Shape, Vec2D _Coord0, Vec2D _Coord1, float _Duration, Color _color);

	void ToggleDbgRender() { m_Show = !m_Show; }
	bool GetShowState() { return m_Show; }

	void Render();

};

