#include "pch.h"
#include "CFSM.h"
#include "CState.h"

CFSM::CFSM(wstring _name)
	: CComponent(_name)
	, m_mapState{}
	, m_CurState(nullptr)
	, m_PrevState(nullptr)
{
}

CFSM::~CFSM()
{
}

void CFSM::AddState(const int& _Key, CState* _State)
{
	assert(m_mapState.find(_Key) == m_mapState.end());
	m_mapState.insert(make_pair(_Key, _State));
	_State->m_Owner = this;
}

CState* CFSM::FindState(const int& _Key)
{
	map<int, CState*>::iterator iter = m_mapState.find(_Key);
	if (iter != m_mapState.end())
		return iter->second;
	else
		return nullptr;
}

void CFSM::ChangeState(const int& _NextState)
{
	CState* pState = FindState(_NextState);
	if (pState)
	{
		m_PrevState = m_CurState;
		m_CurState = pState;
		m_PrevState->Exit();
		m_CurState->Enter();
	}
}

void CFSM::ChangeState(CState* pState)
{
	if (pState)
	{
		m_PrevState = m_CurState;
		m_CurState = pState;
		m_PrevState->Exit();
		m_CurState->Enter();
	}
}

void CFSM::FinalTick()
{
	if (m_CurState == nullptr)
		return;
	m_CurState->FinalTick();
}
