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

struct Directive
{
    EditorTile::Type type;
    int x;
    int y;
};

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
    void mZoomIn();
    void mZoomOut();
    void SetGoPlay(bool goPlay) { m_goPlay = goPlay; }
    bool GetGoPlay() { return m_goPlay; }
    void Copy(int x, int y, bool origin);
    void Paste(int x, int y);

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
    int m_spawnX {-1};
    int m_spawnY {-1};

    bool m_resetCamera {false};

    bool m_goToMainMenu {false};

    bool m_extending {false};

    bool m_areaPlacing {false};

    int m_areaOriginX {-1};
    int m_areaOriginY {-1};

    int m_currentRecDepth {0};

    void PlaceBox(int lowerX, int lowerY, int upperX, int upperY, EditorTile::Type type, int partIdx);
    void Fill(int x, int y, EditorTile::Type type, int partIdx, bool origin);

    bool m_goPlay;

    std::vector<Directive> m_clipboard;
};


inline void EditorScene::GoToMainMenu()
{
    m_goToMainMenu = true;
}