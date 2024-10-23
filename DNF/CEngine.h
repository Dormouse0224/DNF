
class CTexture;

class CEngine
{
	SINGLETON(CEngine);

private:
	HINSTANCE m_hInst;
	HWND m_hMainWnd;
	HDC m_hMainDC;
	Vec2D m_Resolution;
	Vec2D m_WindowSize;
	float m_ScreenScale;
	Vec2D m_MainWndPos;

	wstring m_ResourcePath;

	HDC	m_hSubDC;
	HBITMAP m_hSubBitmap;
	CTexture* m_Backbuffer;

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
	Vec2D GetWindowSize() { return m_WindowSize; }

	HINSTANCE GetProgramInst() { return m_hInst; }
	wstring GetResourcePathW() { return m_ResourcePath; }
	string GetResourcePathA() { return string(m_ResourcePath.begin(), m_ResourcePath.end()); }

public:
	int Init(HINSTANCE _hInst);
	void Progress();
	void CreateBackBuffer();

	void CreateGDIObject();

	void ChangeWindowSize(Vec2D _vResolution);


	//HDC GetSubDC() { return m_hSubDC; }
	HDC GetMainDC() { return m_hMainDC; }
	//Bitmap*	GetSubBitmap() { return m_SubBitmap; }
	CTexture* GetBackbuffer() { return m_Backbuffer; }
};
