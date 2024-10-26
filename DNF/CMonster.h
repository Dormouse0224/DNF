#pragma once
#include "CObj.h"

typedef void (*AttackCallback0)(void);

class CAlbumPlayer;
class CCollider;

class CMonster :
    public CObj
{
public:
    CMonster(wstring _name);
    ~CMonster();

private:
    AttackCallback0 m_AttackCallback;
    vector<CAlbumPlayer*> m_Animation[(int)MonsterState::END];
    int m_DetectRange;
    int m_AttackRange;
    pair<int, int> m_AttackFrame;
    CCollider* m_AttackCol;

public:
    // �浹 �Լ�
    virtual void BeginOverlap(CCollider* _Self, CCollider* _Other);
    virtual void Overlap(CCollider* _Self, CCollider* _Other);
    virtual void EndOverlap(CCollider* _Self, CCollider* _Other);

    virtual void Begin();       // ó�� ������ �� �� ��
    virtual void Tick();    // �ݺ������� �� ��
    virtual void Render();      // ������Ʈ�� �׸���

    AttackCallback0 GetAttackCallback() { return m_AttackCallback; }
    void SetAttackCallback(AttackCallback0 _callback) { m_AttackCallback = _callback; }
    void AddAnimation(MonsterState _state, CAlbumPlayer* _animation);
    int GetDetectRange() { return m_DetectRange; }
    void SetDetectRange(int _range) { m_DetectRange = _range; }
    int GetAttackRange() { return m_AttackRange; }
    void SetAttackRange(int _range) { m_AttackRange = _range; }
    vector<CAlbumPlayer*> GetCurAnimation() { return m_Animation[GetState()]; }
    pair<int, int> GetAttackFrame() { return m_AttackFrame; }
    void SetAttackFrame(pair<int, int> _pair) { m_AttackFrame = _pair; }
    CCollider* GetAttackCol() { return m_AttackCol; }
    void SetAttackCol(CCollider* _col) { m_AttackCol = _col; }
};

