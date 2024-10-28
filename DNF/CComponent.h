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

	// ������ �ȿ��� ������Ʈ�� �� ��
	virtual void FinalTick() = 0;

	virtual void EraseFromLevelVec() {}
};

