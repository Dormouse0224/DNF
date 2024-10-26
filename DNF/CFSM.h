#pragma once
#include "CComponent.h"

class CState;

class CFSM :
    public CComponent
{
public:
    CFSM(wstring _name);
    ~CFSM();
private:
    map<int, CState*>   m_mapState;
    CState* m_CurState;
    CState* m_PrevState;

public:
    void AddState(const int& _Key, CState* _State);
    CState* FindState(const int& _Key);
    void ChangeState(const int& _NextState);
    void ChangeState(CState* pState);

    CState* GetCurState() { return m_CurState; }
    CState* GetPrevState() { return m_PrevState; }

public:
    virtual void FinalTick() override;


};

