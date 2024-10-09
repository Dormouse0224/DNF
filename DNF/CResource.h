#pragma once
#include "CBase.h"

class CResource :
	public CBase
{
public:
	CResource(wstring _Name, ResourceType _ResourceType);
	virtual ~CResource();

private:
	const ResourceType m_ResourceType;

public:
	virtual int Load() = 0;

};

