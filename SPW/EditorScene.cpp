#include "EditorScene.h"
#include "MainCamera.h"
#include "DebugCamera.h"
#include "Background.h"
#include "Button.h"
#include "EditorMap.h"
#include "StaticMap.h"
#include "EditorParser.h"


EditorScene::EditorScene(SDL_Renderer* renderer, RE_Timer& mainTime, const LevelData& levelData):
Scene(renderer, mainTime, ThemeID::SKY), m_camIndex(0), m_cameras(), m_staticMap(*this, 2000, 2000), m_levelData(levelData)
{
    AssetManager &assets = this->GetAssetManager();
    
    m_inputManager.GetApplication().SetEnabled(true);
    m_inputManager.GetMouse().SetEnabled(true);
    m_inputManager.GetControls().SetEnabled(true);
    m_inputManager.GetDebug().SetEnabled(true);
    

    m_cameras[0] = new EditorCamera(*this);
    m_activeCam = m_cameras[m_camIndex];

    EditorParser parser(m_levelData.path);
    parser.InitScene(*this, m_staticMap);

    // Canvas
    m_canvas = new EditorCanvas(*this);

    // Ui
    m_ui = new EditorUi(*this);

    // Background
    Background* background = new Background(*this, Layer::BACKGROUND);
    std::vector<SDL_Texture*> m_textures = m_assetManager.GetBackgrounds();
    switch (m_levelData.themeID)
    {
    case ThemeID::LAKE:
        {
            PE_Vec2 worldDim(24.0f, 24.0f * 1080.0f / 1920.0f);
            background->SetWorldDimensions(worldDim);
            float factors[] = { 0.0f, 0.05f, 0.3f, 0.6f, 0.7f };
            for (int i = 0; i < 5; i++)
            {
                background->SetTexture(i, m_textures[i], PE_Vec2(factors[i], factors[i]));
            }
            break;
        }

    case ThemeID::SKY:
        {
            PE_Vec2 worldDim(24.0f, 24.0f * 1080.0f / 1920.0f);
            background->SetWorldDimensions(worldDim);
            float factors[] = { 0.0f, 0.05f, 0.1f, 0.2f, 0.35f, 0.5f, 0.7f };
            for (int i = 0; i < 7; i++)
            {
                background->SetTexture(i, m_textures[i], PE_Vec2(factors[i], factors[i]));
            }
            break;
        }

    case ThemeID::MOUNTAINS:
    default:
        {
            PE_Vec2 worldDim(36.0f, 36.0f * 1080.0f / 2880.0f);
            background->SetWorldDimensions(worldDim);
            float factors[] = { 0.0f, 0.05f, 0.3f, 0.6f };
            for (int i = 0; i < 4; i++)
            {
                background->SetTexture(i, m_textures[i], PE_Vec2(factors[i], factors[i]));
            }

            Background *foreground = new Background(*this, Layer::FOREGROUND);
            worldDim.Set(36.0f, 36.0f * 400.0f / 2880.0f);
            foreground->SetWorldDimensions(worldDim);
            foreground->SetTexture(0, m_textures[4], PE_Vec2(1.4f, 1.4f));
            break;
        }
    }

    m_editorSaver = new EditorSaver(*this, m_staticMap);
}

EditorScene::~EditorScene()
{
}

bool EditorScene::Update()
{
    bool quit = Scene::Update();

    ApplicationInput &appInput = m_inputManager.GetApplication();
    MouseInput &mouseInput = m_inputManager.GetMouse();
    ControlsInput &controlsInput = m_inputManager.GetControls();
    PE_Vec2 viewPos {mouseInput.viewPos};
    PE_Vec2 worldPos {}; m_activeCam->ViewToWorld(mouseInput.viewPos.x, mouseInput.viewPos.y, worldPos);
    
    if (appInput.quitPressed)
    {
        return true;
    }

    /* --- SAVE --- */
    if(controlsInput.savePressed)
    {
        m_editorSaver->SaveMap(m_levelData.path);
        controlsInput.savePressed = false;
    }
    
    /* --- TILES PLACE --- */
    if(mouseInput.leftDown and not m_ui->IsOverButtons(viewPos.x, viewPos.y))
    {   if((m_ui->GetCurrentTileType() != EditorTile::Type::SPAWN_POINT or not m_spawnSet) and m_ui->GetCurrentTileType() !=m_staticMap.GetTileType((int)worldPos.x, (int)worldPos.y)){
            m_staticMap.SetTile((int)worldPos.x, (int)worldPos.y, m_ui->GetCurrentTileType(), m_ui->GetCurrentPartIdx());
            m_staticMap.InitTiles();
        }
        if(m_ui->GetCurrentTileType() == EditorTile::Type::SPAWN_POINT)
        {
            m_spawnSet = true;
        }
        
    }
    else if(mouseInput.rightDown and not m_ui->IsOverButtons(viewPos.x, viewPos.y))
    {
        if(m_staticMap.GetTileType((int)worldPos.x, (int)worldPos.y) == EditorTile::Type::SPAWN_POINT)
        {
            m_spawnSet = false;
        }
        if(m_ui->GetCurrentTileType() != EditorTile::Type::EMPTY)
        {
            m_staticMap.SetTile((int)worldPos.x, (int)worldPos.y, EditorTile::Type::EMPTY, 0);
            m_staticMap.InitTiles();
        }
    }

    /* --- CAMERA MOVE USING ARROWS --- */
    PE_AABB worldView = m_activeCam->GetWorldView();
    PE_AABB worldBounds = m_activeCam->GetWorldBounds();
    if (m_inputManager.GetControls().goDownDown and worldBounds.lower.y <= worldView.lower.y -0.1)
    {
        PE_Vec2 transl {0.0f, -0.1f};
        m_activeCam->TranslateWorldView(transl);
    }
    if (m_inputManager.GetControls().goUpDown)
    {
        PE_Vec2 transl {0.0f, 0.1f};
        m_activeCam->TranslateWorldView(transl);
    }
    if (m_inputManager.GetControls().goLeftDown and worldBounds.lower.x <= worldView.lower.x -0.1)
    {
        PE_Vec2 transl {-0.1f, 0.0f};
        m_activeCam->TranslateWorldView(transl);
    }
    if (m_inputManager.GetControls().goRightDown)
    {
        PE_Vec2 transl {0.1f, 0.0f};
        m_activeCam->TranslateWorldView(transl);
    }
    
    if(m_resetCamera)
    {
        PE_Vec2 transl {-worldView.lower.x, -worldView.lower.y};
        m_activeCam->TranslateWorldView(transl);
    }
    if(worldView.lower.y <= 0.0f and worldView.lower.x <= 0.0f)
    {
        m_resetCamera = false;
    }
    
    m_mode = UpdateMode::EDITOR;
    if(m_goToMainMenu) Quit();
    return quit;
}

void EditorScene::OnRespawn()
{
}

void EditorScene::ClearGameArea()
{
    for(int x=0; x<m_staticMap.GetRealWidth(); x++)
    {
        for(int y=0; y<m_staticMap.GetRealWidth(); y++)
        {
            EditorTile::Type from_type = m_staticMap.GetTileType(x, y);
            if(from_type != EditorTile::Type::EMPTY) m_staticMap.SetTile(x, y, EditorTile::Type::EMPTY, 0);
        }
    }
}

void EditorScene::ResetCamera()
{
    m_resetCamera = true;
}

void EditorScene::Rollback()
{
    m_staticMap.Rollback(1);
}

