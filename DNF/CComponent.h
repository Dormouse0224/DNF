#pragma once

#include "CBase.h"
class CObj;

class CComponent :
	public CBase
{
	friend CObj;
public:
	CComponent(wstring _ComponentName);
	virtual ~CComponent();

private:
	CObj* m_Owner;

public:
	CObj* GetOwner() { return m_Owner; }

	// 프레임 안에서 컴포넌트가 할 일
	virtual void FinalTick() = 0;

	virtual void EraseFromLevelVec() {}
};

