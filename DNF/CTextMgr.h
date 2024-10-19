#pragma once
class CTextMgr
{
	SINGLETON(CTextMgr);
private:
	wstring m_FontName;
	wstring m_FontPath;
	vector<TextInfo> m_VecTextInfo;
	HDC m_TextDC;
	Font* m_Font;

public:
	void Init();
	void Render(UINT _x, UINT _y, wstring _Text, Color _Color, bool bCameraFallow = false);
	void ClearText() {	m_VecTextInfo.clear(); }
};

