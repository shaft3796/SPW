#pragma once

#include "Settings.h"
#include "GameBody.h"

class Oneway : public GameBody
{
public:
    Oneway(Scene &scene);
    virtual ~Oneway();
    
    virtual void Start() override;
    virtual void Render() override;
    virtual void OnCollisionEnter(GameCollision &collision) override;
    virtual void OnRespawn() override;

private:
    RE_Animator m_animator;
};