#include "EditorScene.h"
#include "MainCamera.h"
#include "DebugCamera.h"
#include "Background.h"
#include "Button.h"
#include "EditorMap.h"
#include "StaticMap.h"


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
    
    PE_AABB bounds(0.0f, 0.0f, (float)10, 24.0f * 9.0f / 16.0f);
    Camera* camera = this->GetActiveCamera();
    camera->SetWorldBounds(bounds);

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
    if(mouseInput.leftDown)
    {
        m_staticMap.SetTile((int)worldPos.x, (int)worldPos.y, m_ui->GetCurrentTileType());
    }
    else if(mouseInput.rightDown)
    {
        m_staticMap.SetTile((int)worldPos.x, (int)worldPos.y, EditorTile::Type::EMPTY);
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
    
    m_mode = UpdateMode::EDITOR;
    return quit;
}

void EditorScene::OnRespawn()
{
}


