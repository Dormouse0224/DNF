#pragma once
class CTextMgr
{
	SINGLETON(CTextMgr);
private:
	wstring m_FontName;
	wstring m_FontPath;
	vector<TextInfo> m_VecTextInfo;
	HDC m_TextDC;
	HFONT m_Font;

public:
	void Init();
	void Render();
	void WriteText(UINT _x, UINT _y, wstring _Text, COLORREF _Color);
	void WriteText(TextInfo _info);
	void DeleteText(UINT _x, UINT _y, wstring _Text);
	void DeleteText(TextInfo _info);
	void ClearText() {	m_VecTextInfo.clear(); }
};

