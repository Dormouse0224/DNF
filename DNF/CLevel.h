#pragma once
#include "CBase.h"
#include "CObj.h"

class CPlayer;
class CCollider;
class CSticker;

class CLevel :
    public CBase
{
public:
    CLevel(wstring _Name);
    virtual ~CLevel();

private:
    vector<CObj*> m_hObj[(int)LayerType::END];
    vector<CCollider*> m_hCollider[(int)LayerType::END];
    vector<CSticker*> m_hSticker;

    CObj* m_SelectedObj;
    CPlayer* m_Player;

public:
    virtual void Begin();
    virtual void Tick();
    virtual void FinalTick();
    virtual void Render();
    virtual void End() = 0;

    void AddObject(CObj* _Object, LayerType _layer) { m_hObj[(int)_layer].push_back(_Object); _Object->m_LayerType = _layer; }
    // Dead 상태인 오브젝트를 제거
    void DeleteObject(LayerType _layer, UINT _ObjectID);
    const vector<CObj*>& GetObjLayer(LayerType _layer) { return m_hObj[(int)_layer]; }

    void AddCollider(CCollider* _Collider, LayerType _layer) { m_hCollider[(int)_layer].push_back(_Collider); }
    const vector<CCollider*>& GetCollider(LayerType _layer) 
    { 
        return m_hCollider[(int)_layer]; 
    }
    void EraseCollider(int _ID, LayerType _Layer);

    void EraseSticker(int _ID);

    void AddSticker(CSticker* _Sticker) { m_hSticker.push_back(_Sticker); }
    const vector<CSticker*>& GetStickerVec() { return m_hSticker; }
    void SetSelectedObj(CObj* _obj) { m_SelectedObj = _obj; }
    CObj* GetSelectedObj() { return m_SelectedObj; }
    void DeleteSelectedObj();

    CPlayer* GetPlayer() { return m_Player; }
    void SetPlayer(CPlayer* _player) { m_Player = _player; }

    // 레벨에 있는 모든 객체를 삭제
    void ClearAll();
};

