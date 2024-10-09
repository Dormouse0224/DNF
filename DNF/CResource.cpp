#include "pch.h"
#include "CResource.h"

CResource::CResource(wstring _Name, ResourceType _ResourceType)
	: CBase(_Name)
	, m_ResourceType(_ResourceType)
{
}

CResource::~CResource()
{
}
