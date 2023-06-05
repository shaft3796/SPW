#pragma once

#include "Settings.h"
#include "Collectable.h"

class Firefly : public Collectable
{
public:
    Firefly(Scene &scene);

    virtual void Start() override;
    virtual void Render() override;
    virtual void OnRespawn() override;

    virtual void Collect(GameBody *collector) override;

private:
    RE_Animator m_animator;
};