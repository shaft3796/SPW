#include "LevelCanvas.h"
#include "LevelScene.h"
#include "LevelHeader.h"

LevelCanvas::LevelCanvas(LevelScene &scene) :
    UIObject(scene), m_levelScene(scene), m_pauseMenu(nullptr), m_endMenu(nullptr)
{
    m_name = "LevelCanvas";
    SetToRespawn(true);
    SetGizmosFillOpacity(0);

    SDL_Renderer *renderer = m_scene.GetRenderer();
    int width, height;
    SDL_RenderGetLogicalSize(renderer, &width, &height);

    m_rect.offsetMin.Set(0.0f, 0.0f);
    m_rect.offsetMax.Set((float)width, (float)height);
    m_rect.anchorMin.Set(0.0f, 0.0f);
    m_rect.anchorMax.Set(0.0f, 0.0f);

    LevelHeader *header = new LevelHeader(scene);
    header->SetParent(this);
}

void LevelCanvas::Update()
{
    if (m_pauseMenu || m_endMenu)
    {
        SetVisible(true);
    }
}

void LevelCanvas::OnRespawn()
{
    SetToRespawn(true);
    ClosePauseMenu();
}

void LevelCanvas::Render()
{
    if (m_pauseMenu == nullptr && m_endMenu == nullptr)
        return;

    SDL_Renderer *renderer = m_scene.GetRenderer();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, NULL);
}

void LevelCanvas::OpenPauseMenu()
{
    if (m_pauseMenu == nullptr)
    {
        m_pauseMenu = new PauseMenu(m_levelScene);
        m_pauseMenu->SetParent(this);
    }
}

void LevelCanvas::OpenEndMenu()
{
    if (m_endMenu == nullptr)
    {
        m_endMenu = new EndMenu(m_levelScene);
        m_endMenu->SetParent(this);
    }
}

void LevelCanvas::ClosePauseMenu()
{
    if (m_pauseMenu)
    {
        m_pauseMenu->Delete();
        m_pauseMenu = nullptr;
    }
}
