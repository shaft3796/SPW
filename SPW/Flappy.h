#pragma once

#include "Settings.h"
#include "Enemy.h"

class Flappy : public Enemy
{
public:
    Flappy(Scene &scene);
    virtual ~Flappy();

    virtual void Start() override;
    virtual void FixedUpdate() override;
    virtual void Render() override;
    virtual void OnRespawn() override;

    virtual void Damage(GameBody *damager) override;

    virtual void OnCollisionStay(GameCollision &collision) override;

private:
    RE_Animator m_animator;

    enum class State
    {
        IDLE, DYING
    };
    State m_state;
    float m_distance;
    float m_speed;
};