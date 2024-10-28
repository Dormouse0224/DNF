#pragma once
#include "CComponent.h"

class CCollider :
    public CComponent
{
public:
    CCollider(wstring _Name);
    ~CCollider();

private:
    Vec2D m_Offset;
    Vec2D m_Location;
    Vec2D m_Size;
    LayerType m_LayerType;

    bool m_IsActive;

    UINT m_OverlapCount;

public:
    void SetSize(Vec2D _Size) { m_Size = _Size; }
    Vec2D GetSize() { return m_Size; }
    void SetOffset(Vec2D _Offset) { m_Offset = _Offset; }
    Vec2D GetOffset() { return m_Offset; }
    Vec2D GetLocation() { return m_Location; }
    LayerType GetLayerType() { return m_LayerType; }
    void SetActiveState(bool _state) { m_IsActive = _state; }
    bool GetActiveState() { return m_IsActive; }

    void BeginOverlap(CCollider* _Other);   // 다른 충돌체와 최초 충돌 시
    void Overlap(CCollider* _Other);        // 다른 충돌체와 BeginOverlap 이후에도 계속 겹쳐있을 경우
    void EndOverlap(CCollider* _Other);     // 다른 충돌체와 겹쳐있다가 최초로 떨어지는 순간

    virtual void FinalTick() override;

    virtual void EraseFromLevelVec() override;
};

