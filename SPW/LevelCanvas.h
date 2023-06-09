#pragma once

#include "Settings.h"
#include "UIObject.h"
#include "PauseMenu.h"
#include "EndMenu.h"

class LevelScene;

class LevelCanvas : public UIObject
{
public:
    LevelCanvas(LevelScene &scene);

    virtual void Update() override;
    virtual void OnRespawn() override;
    virtual void Render() override;

    void OpenEndMenu();
    
    void OpenPauseMenu();
    void ClosePauseMenu();

private:
    EndMenu *m_endMenu;
    PauseMenu *m_pauseMenu;
    LevelScene &m_levelScene;
};