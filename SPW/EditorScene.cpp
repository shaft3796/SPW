#include "EditorScene.h"
#include "MainCamera.h"
#include "DebugCamera.h"
#include "Background.h"

EditorScene::EditorScene(SDL_Renderer* renderer, RE_Timer& mainTime, const LevelData& level):
Scene(renderer, mainTime, level.themeID), m_camIndex(0), m_cameras()
{
    m_inputManager.GetApplication().SetEnabled(true);
    m_inputManager.GetMouse().SetEnabled(true);
    m_inputManager.GetControls().SetEnabled(true);
    m_inputManager.GetDebug().SetEnabled(true);

    // TODO: créer un curseur ?

    m_cameras[0] = new EditorCamera(*this);
    m_activeCam = m_cameras[m_camIndex];

    // TODO: Parseur de niveau

    // Canvas
    m_canvas = new EditorCanvas(*this);

    // Background
    Background* background = new Background(*this, Layer::BACKGROUND);
    std::vector<SDL_Texture*> m_textures = m_assetManager.GetBackgrounds();
    // TODO: customizable background
    switch (level.themeID)
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
        
}

EditorScene::~EditorScene()
{
}

bool EditorScene::Update()
{
    bool quit = Scene::Update();

    ApplicationInput &appInput = m_inputManager.GetApplication();
    
    if (appInput.quitPressed)
    {
        return true;
    }
    
}

void EditorScene::OnRespawn()
{
}


