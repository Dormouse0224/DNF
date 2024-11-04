#include "pch.h"
#include "CLargo_Income.h"
#include "CObj.h"
#include "CLargo.h"

CLargo_Income::CLargo_Income()
	: CState(L"Largo_Income")
{
}

CLargo_Income::~CLargo_Income()
{
}

void CLargo_Income::Enter()
{
	GetOwnerObj()->SetState((int)LargoState::Income);
}

void CLargo_Income::FinalTick()
{
	if (((CLargo*)GetOwnerObj())->IsIncomeFin())
	{
		GetFSM()->ChangeState((int)LargoState::Attack);
	}
}

void CLargo_Income::Exit()
{
}
