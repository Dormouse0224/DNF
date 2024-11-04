#pragma once
#include "CComponent.h"
class CRigidBody :
    public CComponent
{
public:
    CRigidBody(wstring _name);
    ~CRigidBody();

private:
    Vec2D m_Speed;
    bool m_IsAirborne;
    float m_AirborneHeight;
    float m_AirborneSpeed;

    
public:
    virtual void FinalTick();

    bool GetAirborne() { return m_IsAirborne; }
    void SetAirborne(bool _tf) { m_IsAirborne = _tf; }
    float GetAirborneHeight() { return m_AirborneHeight; }

    void AddSpeed(Vec2D _spd) { m_Speed = m_Speed + _spd; }
    Vec2D GetSpeed() { return m_Speed; }
    void SetSpeed(Vec2D _spd) { m_Speed = _spd; }

    void Jump(float _power);
};

