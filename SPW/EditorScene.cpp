#include "EditorScene.h"
#include "MainCamera.h"
#include "DebugCamera.h"
#include "Background.h"
#include "Button.h"
#include "EditorMap.h"
#include "StaticMap.h"
#include "EditorParser.h"


EditorScene::EditorScene(SDL_Renderer* renderer, RE_Timer& mainTime, const LevelData& levelData):
Scene(renderer, mainTime, levelData.themeID), m_camIndex(0), m_cameras(), m_staticMap(*this, 2000, 2000), m_levelData(levelData)
{
    m_inputManager.GetApplication().SetEnabled(true);
    m_inputManager.GetMouse().SetEnabled(true);
    m_inputManager.GetControls().SetEnabled(true);
    m_inputManager.GetDebug().SetEnabled(true);

    m_staticMap.SetFactor(1.0f);
    

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
    PE_Vec2 worldPos {}; m_staticMap.ViewToWorld(mouseInput.viewPos.x, mouseInput.viewPos.y, worldPos);

    if(mouseInput.wheel >= 1)
    {
        mZoomOut();
    }
    else if(mouseInput.wheel <= -1)
    {
        mZoomIn();
    }
    
    
    
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
    EditorTile::Type fromTile {m_staticMap.GetTileType((int)worldPos.x, (int)worldPos.y)};
    EditorTile::Type currentTile {m_ui->GetCurrentTileType()};
    int currentPart {m_ui->GetCurrentPartIdx()};
    
    // Update Place mode
    if(not m_areaPlacing and controlsInput.areaDown and currentTile!=EditorTile::Type::SPAWN_POINT) m_areaPlacing = true;
    else if(m_areaPlacing and controlsInput.areaReleased){
        controlsInput.areaReleased = false;
        m_areaPlacing = false;
        m_areaOriginX = -1; m_areaOriginY = -1;
    }
    
    if(mouseInput.leftDown and not m_ui->IsOverButtons(viewPos.x, viewPos.y))
    {
        if(not m_areaPlacing and not controlsInput.fillDown)
        {
            if((currentTile != EditorTile::Type::SPAWN_POINT or not m_spawnSet) and currentTile!=fromTile)
            {
                m_staticMap.SetTile((int)worldPos.x, (int)worldPos.y, currentTile, currentPart, m_extending);
                m_extending = true;
                m_staticMap.InitTiles();
                if(m_ui->GetCurrentTileType() == EditorTile::Type::SPAWN_POINT)
                {
                    m_spawnSet = true; m_spawnX = (int)worldPos.x; m_spawnY = (int)worldPos.y;
                }
            }
        }
        else if(m_areaPlacing)
        {
            // AREA PLACING INIT
            if(m_areaOriginX == -1 and m_areaOriginY == -1)
            {
                m_areaOriginX = (int)worldPos.x;
                m_areaOriginY = (int)worldPos.y;
                m_staticMap.SetTile((int)worldPos.x, (int)worldPos.y, currentTile, currentPart, false);
            }
            else
            {
                Rollback();
                int lowerX = std::min(m_areaOriginX, (int)worldPos.x);
                int lowerY = std::min(m_areaOriginY, (int)worldPos.y);
                int upperX = std::max(m_areaOriginX, (int)worldPos.x);
                int upperY = std::max(m_areaOriginY, (int)worldPos.y);
                PlaceBox(lowerX, lowerY, upperX, upperY, currentTile, currentPart);
                m_staticMap.InitTiles();
            }
        }
        else if(controlsInput.fillDown)
        {
            Fill((int)worldPos.x, (int)worldPos.y, currentTile, currentPart, true);
            m_staticMap.InitTiles();
        }
    }
    else if (mouseInput.rightDown and not m_ui->IsOverButtons(viewPos.x, viewPos.y))
    {
        if(not m_areaPlacing and not controlsInput.fillDown)
        {
            if(fromTile != EditorTile::Type::EMPTY)
            {
                m_staticMap.SetTile((int)worldPos.x, (int)worldPos.y, EditorTile::Type::EMPTY, 0, m_extending);
                m_extending = true;
                m_staticMap.InitTiles();
                if(currentTile == EditorTile::Type::SPAWN_POINT) m_spawnSet = false;
            }
        }
        else if (m_areaPlacing)
        {
            // AREA PLACING INIT
            if(m_areaOriginX == -1 and m_areaOriginY == -1)
            {
                m_areaOriginX = (int)worldPos.x;
                m_areaOriginY = (int)worldPos.y;
                m_staticMap.SetTile((int)worldPos.x, (int)worldPos.y, EditorTile::Type::EMPTY, 0, false);
            }
            else
            {
                Rollback();
                int lowerX = std::min(m_areaOriginX, (int)worldPos.x);
                int lowerY = std::min(m_areaOriginY, (int)worldPos.y);
                int upperX = std::max(m_areaOriginX, (int)worldPos.x);
                int upperY = std::max(m_areaOriginY, (int)worldPos.y);
                PlaceBox(lowerX, lowerY, upperX, upperY, EditorTile::Type::EMPTY, 0);
                m_staticMap.InitTiles();
            }
        }
        else if (controlsInput.fillDown)
        {
            Fill((int)worldPos.x, (int)worldPos.y, EditorTile::Type::EMPTY, 0, true);
            m_staticMap.InitTiles();
        }
        
    }
    
    // RELEASE EXTENDING
    if(mouseInput.leftReleased || mouseInput.rightReleased) m_extending = false;

    /* --- CAMERA MOVE USING ARROWS --- */
    PE_AABB worldView = m_activeCam->GetWorldView();
    float worldViewLowerX = worldView.lower.x/m_staticMap.getFactor();
    float worldViewLowerY = worldView.lower.y/m_staticMap.getFactor();
    float worldViewUpperX = worldView.upper.x/m_staticMap.getFactor();
    float worldViewUpperY = worldView.upper.y/m_staticMap.getFactor();
    float move = 0.15f/m_staticMap.getFactor();
    if (m_inputManager.GetControls().goDownDown and 0 < worldViewLowerY - move)
    {
        PE_Vec2 transl {0.0f, -move};
        m_activeCam->TranslateWorldView(transl);
    }
    if (m_inputManager.GetControls().goUpDown and worldViewUpperY + move < (float)m_staticMap.GetHeight())
    {
        PE_Vec2 transl {0.0f, move};
        m_activeCam->TranslateWorldView(transl);
    }
    if (m_inputManager.GetControls().goLeftDown and 0 < worldViewLowerX - move)
    {
        PE_Vec2 transl {-move, 0.0f};
        m_activeCam->TranslateWorldView(transl);
    }
    if (m_inputManager.GetControls().goRightDown and worldViewUpperX + move < (float)m_staticMap.GetWidth())
    {
        PE_Vec2 transl {move, 0.0f};
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
            bool extend = (y!=0 || x!=0);
            if(from_type != EditorTile::Type::EMPTY) m_staticMap.SetTile(x, y, EditorTile::Type::EMPTY, 0, extend);
            if(from_type == EditorTile::Type::SPAWN_POINT) SetSpawnSet(false);
        }
    }
}

void EditorScene::ResetCamera()
{
    m_resetCamera = true;
}

void EditorScene::Rollback()
{
    m_staticMap.RollbackGroup();
}

void EditorScene::Forward()
{
    m_staticMap.ForwardGroup();
}

void EditorScene::PlaceBox(int lowerX, int lowerY, int upperX, int upperY, EditorTile::Type type, int partIdx)
{
    if(m_spawnSet && lowerX <= m_spawnX && m_spawnX <= upperX && lowerY <= m_spawnY && m_spawnY <= upperY)
    {
        SetSpawnSet(false);
    }
    for(int x=lowerX; x<=upperX; x++)
    {
        for(int y=lowerY; y<=upperY; y++)
        {
            bool extend = x!=lowerX or y!=lowerY;
            m_staticMap.SetTile(x, y, type, partIdx, extend);
        }
    }
}

void EditorScene::Fill(int x, int y, EditorTile::Type type, int partIdx, bool origin)
{
    if(origin) m_currentRecDepth = 0;
    m_currentRecDepth++;
    if(m_currentRecDepth > 2000) return;
    if(m_staticMap.GetTileType(x, y) == type) return;
    m_staticMap.SetTile(x, y, type, partIdx, !origin);
    if(x > 0) Fill(x-1, y, type, partIdx, false);
    if(x < m_staticMap.GetRealWidth()-1) Fill(x+1, y, type, partIdx, false);
    if(y > 0) Fill(x, y-1, type, partIdx, false);
    if(y < m_staticMap.GetRealHeight()-1) Fill(x, y+1, type, partIdx, false);
    
}

void EditorScene::mZoomIn()
{
    float factor = m_staticMap.getFactor();
    factor -= 0.1f;
    if(factor < 0.1f) factor = 0.1f;
    // Test if the camera is not out of the map
    PE_AABB worldView = m_activeCam->GetWorldView();
    float worldViewLowerX = worldView.lower.x/factor;
    float worldViewLowerY = worldView.lower.y/factor;
    float worldViewUpperX = worldView.upper.x/factor;
    float worldViewUpperY = worldView.upper.y/factor;
    if(worldViewLowerX < 0.0f) return;
    if(worldViewLowerY < 0.0f) return;
    if(worldViewUpperX > (float)m_staticMap.GetWidth()) return;
    if(worldViewUpperY > (float)m_staticMap.GetHeight()) return;
    m_staticMap.SetFactor(factor);
}

void EditorScene::mZoomOut()
{
    float factor = m_staticMap.getFactor();
    factor += 0.1f;
    if(factor > 1.0f) factor = 1.0f;
    m_staticMap.SetFactor(factor);
}

