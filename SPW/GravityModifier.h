#pragma once

#include "Settings.h"
#include "Collectable.h"

class GravityModifier : public Collectable
{
public:
    GravityModifier(Scene &scene, float gravity_x, float gravity_y);

    virtual void Start() override;
    virtual void Render() override;
    virtual void OnRespawn() override;

    virtual void OnCollisionEnter(GameCollision &collision) override;


private:
    RE_Animator m_animator;
    float m_gravity_x;
    float m_gravity_y;
};