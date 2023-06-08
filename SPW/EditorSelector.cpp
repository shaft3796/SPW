#include "EditorSelector.h"
#include "EditorScene.h"

EditorSelector::EditorSelector(EditorScene& scene, EditorUi* ui, RE_AtlasPart* atlasPart, int partIdx):
UIObject(scene), m_atlasPart(atlasPart), m_currState(State::IDLE), m_prevState(State::IDLE), m_listener(nullptr), m_partIdx(partIdx), m_ui(ui){

    m_name = "EditorSelector";
}

EditorSelector::~EditorSelector(){
    if (m_listener) delete m_listener;
}

void EditorSelector::Update(){
    SetVisible(true);
    MouseInput &mouse = m_scene.GetInputManager().GetMouse();
    m_prevState = m_currState;

    SDL_FPoint mousePos = { mouse.viewPos.x, mouse.viewPos.y };
    if(Contains(mousePos)){
        if(mouse.leftDown) mouse.leftDown = false;  // Evite la pose de Tile sur les selecteurs
        if(mouse.leftReleased){
            if(m_listener){
                if(m_currState == State::IDLE){
                    m_ui->DeselectAllSelectors();
                    m_currState = State::SELECTED;         
                    m_listener->OnSelect();
                }

                m_scene.GetAssetManager().PlaySound(
                        SoundID::SYSTEM_SELECT, ChannelID::SYSTEM_1
                    );

                mouse.leftReleased = false;
            }
        }
    }

    // TODO: Maybe useless
    if (m_currState != m_prevState && m_listener) m_listener->OnStateChange(m_currState);
}

void EditorSelector::Render(){
    SDL_Renderer *renderer = m_scene.GetRenderer();
    SDL_Texture *texture = NULL;
    SDL_FRect dstRect = GetCanvasRect();

    if (m_atlasPart){
        const SDL_Rect *srcRect = NULL;
        
        if(m_currState == State::SELECTED){
            AssetManager& assets = m_scene.GetAssetManager(); AssertNew(&assets);
            RE_Atlas *terrainAtlas = assets.GetAtlas(AtlasID::TERRAIN); AssertNew(terrainAtlas);
            RE_AtlasPart *part = terrainAtlas->GetPart("BonusEmpty"); AssertNew(part);

            texture = part->GetTexture();
            dstRect = GetCanvasRect();
            dstRect.x -= 8;
            dstRect.y -= 8;
            dstRect.w += 16;
            dstRect.h += 16;
            
            srcRect = part->GetSrcRect(0);

            SDL_RenderCopyF(renderer, texture, srcRect, &dstRect);
        }
        
        texture = m_atlasPart->GetTexture();
        dstRect = GetCanvasRect();
        

        srcRect = m_atlasPart->GetSrcRect(m_partIdx);

        if(m_reverse)
        {
            SDL_FPoint center = {0, 0};
            SDL_RenderCopyExF(renderer, texture, srcRect, &dstRect, 0.0, &center, SDL_FLIP_VERTICAL);
        } else
        {
            SDL_RenderCopyF(renderer, texture, srcRect, &dstRect); 
        }
        
    }
}

EditorSelectorListener::EditorSelectorListener(){

}

EditorSelectorListener::~EditorSelectorListener(){

}

void EditorSelectorListener::OnSelect(){

}

void EditorSelectorListener::OnDeselect(){

}

void EditorSelectorListener::OnStateChange(EditorSelector::State state){

}