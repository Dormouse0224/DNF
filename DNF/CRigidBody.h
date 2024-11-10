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
    float m_GravityReduce;
    bool m_bZeroGravity;

    
public:
    virtual void FinalTick();

    bool GetAirborne() { return m_IsAirborne; }
    void SetAirborne(bool _tf) { m_IsAirborne = _tf; }
    float GetAirborneHeight() { return m_AirborneHeight; }
    void SetAirborneHeight(float _height) { m_AirborneHeight = _height; }
    float GetAirborneSpeed() { return m_AirborneSpeed; }
    void SetAirborneSpeed(float _spd) { m_AirborneSpeed = _spd; }

    void AddSpeed(Vec2D _spd) { m_Speed = m_Speed + _spd; }
    Vec2D GetSpeed() { return m_Speed; }
    void SetSpeed(Vec2D _spd) { m_Speed = _spd; }
    void SetZeroGravity(bool _b) { m_bZeroGravity = _b; }
    void SetGravityReduce(float _g) { m_GravityReduce = _g; }

    void Jump(float _power);
};