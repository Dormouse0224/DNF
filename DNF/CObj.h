#pragma once
#include "CBase.h"

class CComponent;
class CCollider;
class CLevel;
class CAlbumPlayer;

class CObj :
    public CBase
{
    friend CLevel;

public:
    CObj(wstring _Name);
    CObj(wstring _Name, Vec2D _Loc, Vec2D _Scale);
    virtual ~CObj();

private:
    Vec2D m_Location;
    Vec2D m_Scale;
    vector<CComponent*> m_ComponentVector;
    LayerType m_LayerType;
    Vec2D m_GroundPos;

    int m_State;

    bool m_Dead;

protected:
    vector<CAlbumPlayer*> m_AlbumPlayerVector;

public:
    void SetLocation(Vec2D _Location) { m_Location = _Location; m_GroundPos = Vec2D(m_Location.x + (m_Scale.x / 2.f), m_Location.y + m_Scale.y); }
    void SetScale(Vec2D _Scale) { m_Scale = _Scale; m_GroundPos = Vec2D(m_Location.x + (m_Scale.x / 2.f), m_Location.y + m_Scale.y); }
    Vec2D GetLocation() const { return m_Location; }
    Vec2D GetScale() const { return m_Scale; }
    Vec2D GetGroundPos() const { return m_GroundPos; }
    void SetDead(bool _dead) { m_Dead = _dead; }
    bool GetDead() { return m_Dead; }
    int GetState() { return m_State; }
    void SetState(int _state) { m_State = _state; }

    // 컴포넌트
    virtual void AddComponent(CComponent* _Component);
    
    template<typename T>
    T* GetComponent(wstring _ComponentName);

    LayerType GetLayerType() { return m_LayerType; }


    // 충돌 함수
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);


    virtual void Begin();       // 처음 시작할 때 할 일
    virtual void Tick() = 0;    // 반복적으로 할 일
    virtual void Render();      // 오브젝트를 그리기

    virtual void FinalTick() final;   // 오브젝트가 소유한 Component 가 할 일

    virtual void WriteInfo(fstream _stream) {}
};

