#include "EditorHeader.h"
#include "EditorScene.h"
#include "Image.h"

EditorHeader::EditorHeader(EditorScene &scene): UIObject(scene), m_editorScene(scene){
    m_name = "EditorHeader";

    AssetManager &assets = scene.GetAssetManager();
    TTF_Font *font = assets.GetFont(FontID::LARGE);
    SDL_Color color = assets.GetColor(ColorID::NORMAL);
    RE_Atlas *atlas = assets.GetAtlas(AtlasID::UI); // TODO: modify source here

    int w, h;
    TTF_SizeUTF8(font, u8"00", &w, &h);
    float imgH = 80.0f;
    float imgW = imgH;
    float numW = (float)w;
    float sep = 20.0f;
    float panelW = imgW + sep + numW;
    float panelH = imgH;

    m_rect.anchorMin.Set(0.0f, 0.0f);
    m_rect.anchorMax.Set(0.0f, 0.0f);
    m_rect.offsetMin.Set(80.0f, 40.0f);
    m_rect.offsetMax = m_rect.offsetMin + Vec2(panelW, panelH);

    float currX = 0.0f;
    float currY = 0.0f;

}

void EditorHeader::Update(){
    
}