#include "EditorCanvas.h"
#include "EditorScene.h"
#include "EditorHeader.h"

EditorCanvas::EditorCanvas(EditorScene &scene) : UIObject(scene), m_editorScene(scene){
    m_name = "EditorCanvas";
    SetToRespawn(true);
    SetGizmosFillOpacity(0);

    SDL_Renderer *renderer = m_scene.GetRenderer();
    int width, height;
    SDL_RenderGetLogicalSize(renderer, &width, &height);

    m_rect.offsetMin.Set(0.0f, 0.0f);
    m_rect.offsetMax.Set((float)width, (float)height);
    m_rect.anchorMin.Set(0.0f, 0.0f);
    m_rect.anchorMax.Set(0.0f, 0.0f);

    EditorHeader *header = new EditorHeader(scene);
    header->SetParent(this);
}

void EditorCanvas::Update(){
}

void EditorCanvas::OnRespawn(){
    SetToRespawn(true);
}

void EditorCanvas::Render(){
    SDL_Renderer *renderer = m_scene.GetRenderer();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, NULL);
}