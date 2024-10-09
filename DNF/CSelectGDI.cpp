#include "pch.h"
#include "CSelectGDI.h"

CSelectGDI::CSelectGDI(HDC _dc, HGDIOBJ _InputObject)
	: m_DC(_dc)
	, m_PrevObject(nullptr)
{
	m_PrevObject = SelectObject(m_DC, _InputObject);
}

CSelectGDI::~CSelectGDI()
{
	SelectObject(m_DC, m_PrevObject);
}
