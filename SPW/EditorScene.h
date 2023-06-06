#pragma once

#include "Settings.h"
#include "Scene.h"
#include "Player.h"
#include "EditorCanvas.h"
#include "LevelData.h"
#include "Constants.h"

class EditorScene : public Scene{
public:
    /* CONSTRUCTOR */
    EditorScene(SDL_Renderer *renderer, RE_Timer &mainTime, const LevelData &level);
    EditorScene(EditorScene const&) = delete;
    EditorScene& operator=(EditorScene const&) = delete;
    ~EditorScene() override;
    
    bool Update() override;
    void OnRespawn() override;

private:
    std::array<Camera *, 2> m_cameras;
    EditorCanvas *m_canvas;

    PE_Vec2 m_startPos;

    int m_camIndex;
    
};