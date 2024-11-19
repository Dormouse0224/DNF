#pragma once
#include "CObj.h"

typedef void (*NPCReactCallback1)(CNPC*);

class CUI;

class CNPC :
    public CObj
{
public:
    CNPC(wstring _name);
    ~CNPC();

private:
    NPCReactCallback1 m_reactCallback;
    int m_CallbackIndex;
    wstring m_IdleAni;
    CAlbumPlayer* m_IdlePlayer;

    wstring m_TeleportDest;

    CUI* m_NPCUI;

public:
    // �浹 �Լ�
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);


    virtual void Begin();       // ó�� ������ �� �� ��
    virtual void Tick();        // �ݺ������� �� ��
    virtual void Render();      // ������Ʈ�� �׸���


    wstring GetIdleAni() { return m_IdleAni; }
    void SetIdleAni(wstring _animation) { m_IdleAni = _animation; }
    wstring GetTeleportDest() { return m_TeleportDest; }
    void SetTeleportDest(wstring _dest) { m_TeleportDest = _dest; }
    int GetCallbackIndex() { return m_CallbackIndex; }
    void SetCallbackIndex(int i) { m_CallbackIndex = i; }

    void SetIdlePlayer(CAlbumPlayer* _ap) { m_IdlePlayer = _ap; }
    void SetreactCallback(NPCReactCallback1 _callback) { m_reactCallback = _callback; }
    CUI* GetNPCUI() { return m_NPCUI; }
    void SetNPCUI(CUI* _ui) { m_NPCUI = _ui; }

    void InitCallback();
};

