#pragma once
class CTextMgr
{
	SINGLETON(CTextMgr);
private:
	wstring m_FontName;
	wstring m_FontPath;
	Font* m_Font;
	int m_FontHeight;

public:
	void Init();
	void Render(UINT _x, UINT _y, wstring _Text, Color _Color, bool bCameraFallow = false);

	int GetFontHeight() { return m_FontHeight; }
};

