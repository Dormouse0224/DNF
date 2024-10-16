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

    bool m_Dead;

protected:
    vector<CAlbumPlayer*> m_AlbumPlayerVector;

public:
    void SetLocation(Vec2D _Location) { m_Location = _Location; }
    void SetScale(Vec2D _Scale) { m_Scale = _Scale; }
    Vec2D GetLocation() { return m_Location; }
    Vec2D GetScale() { return m_Scale; }
    void SetDead(bool _dead) { m_Dead = _dead; }
    bool GetDead() { return m_Dead; }

    // ������Ʈ
    virtual void AddComponent(CComponent* _Component);
    
    template<typename T>
    T* GetComponent(wstring _ComponentName);

    LayerType GetLayerType() { return m_LayerType; }


    // �浹 �Լ�
    void BeginOverlap(CCollider* _Self, CCollider* _Other);
    void Overlap(CCollider* _Self, CCollider* _Other);
    void EndOverlap(CCollider* _Self, CCollider* _Other);


public:
    virtual void Begin();       // ó�� ������ �� �� ��
    virtual void Tick() = 0;    // �ݺ������� �� ��
    virtual void FinalTick() final;   // ������Ʈ�� ������ Component �� �� ��
    virtual void Render();      // ������Ʈ�� �׸���


};

