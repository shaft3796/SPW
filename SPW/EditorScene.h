#pragma once

#include "Settings.h"
#include "Scene.h"
#include "Player.h"
#include "EditorCanvas.h"
#include "LevelData.h"
#include "Constants.h"
#include "EditorMap.h"
#include "EditorUi.h"
#include "StaticMap.h"
#include "EditorSaver.h"

class EditorScene : public Scene{
public:
    /* CONSTRUCTOR */
    EditorScene(SDL_Renderer *renderer, RE_Timer &mainTime, const LevelData &levelData);
    EditorScene(EditorScene const&) = delete;
    EditorScene& operator=(EditorScene const&) = delete;
    ~EditorScene() override;
    
    bool Update() override;
    void OnRespawn() override;

    EditorSaver* getSaver() { return m_editorSaver; }
    const std::string& getPath() { return m_levelData.path; }

    void SetNoSetTile(bool noSetTile) { m_noSetTile = noSetTile; }
    void SetSpawnSet(bool spawnSet) { m_spawnSet = spawnSet; }

    void ClearGameArea();
    void ResetCamera();
    void GoToMainMenu();
    void Rollback();
    void Forward();

private:
    std::array<Camera *, 2> m_cameras;
    EditorCanvas *m_canvas;
    EditorUi *m_ui;

    PE_Vec2 m_startPos;

    int m_camIndex;

    EditorMap m_staticMap;
    EditorSaver* m_editorSaver;

    const LevelData &m_levelData;

    bool m_noSetTile {false};

    bool m_spawnSet {false};

    bool m_resetCamera {false};

    bool m_goToMainMenu {false};

    bool m_extending {false};

    bool m_areaPlacing {false};

    int m_areaOriginX {0};
    int m_areaOriginY {0};

    void PlaceBox(int lowerX, int lowerY, int upperX, int upperY, EditorTile::Type type, int partIdx);
};


inline void EditorScene::GoToMainMenu()
{
    m_goToMainMenu = true;
}
