#include "pch.h"
#include "CComponent.h"
#include "CObj.h"

CComponent::CComponent(wstring _Name)
	: CBase(_Name)
	, m_Owner(nullptr)
{
}

CComponent::~CComponent()
{
}


