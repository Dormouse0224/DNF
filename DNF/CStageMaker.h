#pragma once
#include "CStage.h"

class CObj;
class CMonster;
class CWall;
class CNPC;
class CPortal;

class CStageMaker :
    public CStage
{
public:
    CStageMaker();
    ~CStageMaker();

private:
    HWND m_hControlPanel;
    HWND m_hSelectedObj;
    HWND m_hUpperBoundSlide;
    HWND m_hUpperBoundStatic;
    int m_UpperBound;

    CPortal* m_PortalArr[4];
    map<int, CMonster*> m_MonsterMap;
    map<int, CWall*> m_WallMap;
    map<int, CNPC*> m_NPCMap;


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;

    map<int, CMonster*>& GetMonsterMap() { return m_MonsterMap; }
    map<int, CWall*>& GetWallMap() { return m_WallMap; }
    map<int, CNPC*>& GetNPCMap() { return m_NPCMap; }

    void EraseObjFromMap(int _ID);
};

