#include "pch.h"
#include "CState.h"

CState::CState(wstring _name)
	: CBase(_name)
	, m_Owner(nullptr)
{
}

CState::~CState()
{
}
