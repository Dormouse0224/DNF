#include "pch.h"

class CEngine
{
	SINGLETON(CEngine);

private:
	HINSTANCE m_hInst;
	HWND m_hMainWnd;
	HDC m_hMainDC;
	Vec2D m_Resolution;
	float m_ScreenScale;
	Vec2D m_MainWndPos;

	HDC m_hSubDC;
	HBITMAP	m_hSubBitmap;

	HPEN		m_Pen[(UINT)PEN_TYPE::END];
	HBRUSH		m_Brush[(UINT)BRUSH_TYPE::END];

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;


public:
	HPEN GetPen(PEN_TYPE _Type) { return m_Pen[(UINT)_Type]; }
	HBRUSH GetBrush(BRUSH_TYPE _Type) { return m_Brush[(UINT)_Type]; }
	Vec2D GetResolution() { return m_Resolution; }
	void SetResolution(Vec2D _Resolution) { m_Resolution = _Resolution; }
	float GetScreenScale() { return m_ScreenScale; }
	void SetScreenScale(float _Scale) { m_ScreenScale = _Scale; }
	HWND GetMainWnd() { return m_hMainWnd; }


public:
	int Init(HINSTANCE _hInst);
	void Progress();
	void CreateDoubleBuffer();

	void CreateGDIObject();

	void ChangeWindowSize(Vec2D _vResolution);


	HDC GetSubDC() { return m_hSubDC; }
	HDC GetMainDC() { return m_hMainDC; }
};
