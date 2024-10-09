#pragma once
class CSelectGDI
{
	private:
	HDC		 m_DC;
	HGDIOBJ	 m_PrevObject;

public:
	CSelectGDI(HDC _dc, HGDIOBJ _InputObject);
	~CSelectGDI();
};

