#include "pch.h"
#include "CBase.h"

UINT CBase::m_NextID = 0;

CBase::CBase(wstring _Name)
	: m_ID(m_NextID++)
	, m_Name(_Name)
{
}

CBase::CBase(const CBase& _Other)
	: m_ID(m_NextID++)
	, m_Name(_Other.m_Name)
{
}

CBase::~CBase()
{
}

void CBase::operator=(const CBase& _Other)
{
	m_Name = _Other.m_Name;
}

