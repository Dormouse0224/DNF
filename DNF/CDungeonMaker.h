#pragma once
#include "CLevel.h"
class CDungeonMaker :
    public CLevel
{
public:
    CDungeonMaker();
    ~CDungeonMaker();

private:
    map<Vec2D, StageInfo*> m_StageInfoMap;
    Vec2D m_SelectedTile;
    const int m_RightBound;

    bool m_bFinalExist;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void End() override;

    void AddStageInfo(Vec2D _GridLocation, StageInfo* _stageinfo);
    StageInfo* FindStageInfo(Vec2D _GridLocation);
    Vec2D GetSelectedTile() { return m_SelectedTile; }

private:
    void ClearStageInfoMap();

    void CreateStageCallback();
    void EditStageCallback();
    void DeleteStageCallback();
    void ToggleFinalCallback();
    void SaveDungeonCallback();
    void LoadDungeonCallback();

};

