#pragma once

class CBase
{
public:
	CBase(wstring _Name);
	CBase(const CBase& _Other);
	virtual ~CBase();
	void operator =(const CBase& _Other);

private:
	static UINT m_NextID;
	wstring m_Name;
	const UINT m_ID;

public:
	const wstring& GetName() { return m_Name; }
	UINT GetID() { return m_ID; }
	
};

