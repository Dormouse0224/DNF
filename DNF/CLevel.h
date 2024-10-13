#pragma once
#include "CBase.h"
#include "CObj.h"

class CCollider;

class CLevel :
    public CBase
{
public:
    CLevel(wstring _Name);
    virtual ~CLevel();

private:
    vector<CObj*> m_hObj[(int)LayerType::END];
    vector<CCollider*> m_hCollider[(int)LayerType::END];

public:
    virtual void Begin();
    virtual void Tick();
    virtual void FinalTick();
    virtual void Render();
    virtual void End() = 0;

    void AddObject(CObj* _Object, LayerType _layer) { m_hObj[(int)_layer].push_back(_Object); _Object->m_LayerType = _layer; }
    void DeleteObject();
    vector<CObj*> GetObjLayer(LayerType _layer) { return m_hObj[(int)_layer]; }

    void AddCollider(CCollider* _Collider, LayerType _layer) { m_hCollider[(int)_layer].push_back(_Collider); }
    const vector<CCollider*>& GetCollider(LayerType _layer) { return m_hCollider[(int)_layer]; }

};

