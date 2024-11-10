#pragma once
#include "CBase.h"

class CComponent;
class CCollider;
class CLevel;
class CAlbumPlayer;
class CRigidBody;

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
    bool m_GroundPosInit;

    CCollider* m_BodyCollider;
    CRigidBody* m_RigidBody;

    int m_State;

    bool m_Dead;

protected:
    vector<CAlbumPlayer*> m_AlbumPlayerVector;
    bool m_bLookLeft;
    bool m_bFallowCam;
    int m_HP;

public:
    void SetLocation(Vec2D _Location);
    void RenewGroundPos(Vec2D _Location) { m_GroundPos = _Location; }
    void SetGroundPos(Vec2D _Location);
    void SetScale(Vec2D _Scale);
    Vec2D GetLocation() const { return m_Location; }
    Vec2D GetScale() const { return m_Scale; }
    Vec2D GetGroundPos() const { return m_GroundPos; }
    void SetDead(bool _dead) { m_Dead = _dead; }
    bool GetDead() { return m_Dead; }
    int GetState() { return m_State; }
    void SetState(int _state) { m_State = _state; }
    bool IsLookLeft() { return m_bLookLeft; }
    void SetLookLeft(bool _b) { m_bLookLeft = _b; }
    bool GetFallowCam() { return m_bFallowCam; }
    void SetFallowCam(bool _b) { m_bFallowCam = _b; }
    void GiveDamage(int _dmg);

    // 컴포넌트
    virtual void AddComponent(CComponent* _Component);

    CCollider* GetBodyCollider() { return m_BodyCollider; }
    void RegisterBodyCollider(CCollider* _Coll);
    CRigidBody* GetRigidBody() { return m_RigidBody; }
    void SetRigidBody(CRigidBody* _RB);
    
    template<typename T>
    T* GetComponent(wstring _ComponentName);

    LayerType GetLayerType() { return m_LayerType; }
    void SetLayerType(LayerType _layertype) { m_LayerType = _layertype; }


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

