#pragma once
class CTextMgr
{
	SINGLETON(CTextMgr);
private:
	wstring m_FontName;
	wstring m_FontPath;
	vector<TextInfo> m_VecTextInfo;

public:
	void Init();
	void Render();
	void WriteText(int _Height, int _Weight, UINT _x, UINT _y, wstring _Text, COLORREF _Color);
	void DeleteText(UINT _x, UINT _y, wstring _Text);
	void ClearText() {	m_VecTextInfo.clear(); }
};

