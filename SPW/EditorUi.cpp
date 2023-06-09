#include "EditorUi.h"

#include "Button.h"
#include "EditorScene.h"
#include "EditorSelector.h"

class EditorSelector;

namespace EditorUiNS
{
    class NsEditorSelectorListener : public EditorSelectorListener{
    public:
        NsEditorSelectorListener(EditorScene &editorScene, EditorUi &editorUi, EditorTile::Type tileType, int partIdx) : m_editorScene(editorScene), m_editorUi(editorUi), m_tileType(tileType), m_partIdx(partIdx){}
        virtual ~NsEditorSelectorListener(){}

        virtual void OnSelect() override{
            m_editorUi.SetCurrentTileType(m_tileType);
            m_editorUi.SetCurrentPartIdx(m_partIdx);
        }

        virtual void OnDeselect() override{
            
        }

    private:
        EditorScene &m_editorScene;
        EditorUi &m_editorUi;
        EditorTile::Type m_tileType;
        int m_partIdx;
    };

    class SaveListener : public ButtonListener
    {
    public:
        SaveListener(EditorScene &editorScene, EditorUi &editorUi) : m_editorScene(editorScene), m_editorUi(editorUi){}

        virtual void OnPress() override
        {
            m_editorScene.getSaver()->SaveMap(m_editorScene.getPath());
            m_editorScene.SetNoSetTile(true);
        }

    private:
        EditorScene &m_editorScene;
        EditorUi &m_editorUi;
    };

    class ClearListener : public ButtonListener
    {
    public:
        ClearListener(EditorScene &editorScene, EditorUi &editorUi) : m_editorScene(editorScene), m_editorUi(editorUi){}

        virtual void OnPress() override
        {
            m_editorScene.ClearGameArea();
        }

    private:
        EditorScene &m_editorScene;
        EditorUi &m_editorUi;
    };
    class CenterListener : public ButtonListener
    {
    public:
        CenterListener(EditorScene &editorScene, EditorUi &editorUi) : m_editorScene(editorScene), m_editorUi(editorUi){}

        virtual void OnPress() override
        {
            m_editorScene.ResetCamera();
        }

    private:
        EditorScene &m_editorScene;
        EditorUi &m_editorUi;
    };

    class TitleListener : public ButtonListener
    {
    public:
        TitleListener(EditorScene &editorScene, EditorUi &editorUi) : m_editorScene(editorScene), m_editorUi(editorUi){}

        virtual void OnPress() override
        {
            m_editorScene.GoToMainMenu();
        }

    private:
        EditorScene &m_editorScene;
        EditorUi &m_editorUi;
    };

    class RollbackListener : public ButtonListener
    {
    public:
        RollbackListener(EditorScene &editorScene, EditorUi &editorUi) : m_editorScene(editorScene), m_editorUi(editorUi){}

        virtual void OnPress() override
        {
            m_editorScene.Rollback();
        }

    private:
        EditorScene &m_editorScene;
        EditorUi &m_editorUi;
    };

    class ForwardListener : public ButtonListener
    {
    public:
        ForwardListener(EditorScene &editorScene, EditorUi &editorUi) : m_editorScene(editorScene), m_editorUi(editorUi){}

        virtual void OnPress() override
        {
            m_editorScene.Forward();
        }

    private:
        EditorScene &m_editorScene;
        EditorUi &m_editorUi;
    };

    class PlayListener : public ButtonListener
    {
    public:
        PlayListener(EditorScene &editorScene, EditorUi &editorUi) : m_editorScene(editorScene), m_editorUi(editorUi){}

        virtual void OnPress() override
        {
            
            m_editorScene.SetGoPlay(true);
            m_editorScene.getSaver()->SaveMap(m_editorScene.getPath());
            m_editorScene.SetNoSetTile(true);
            m_editorScene.GoToMainMenu();
            
        }

    private:
        EditorScene &m_editorScene;
        EditorUi &m_editorUi;
    };
    
}

EditorUi::EditorUi(EditorScene& scene): UIObject(scene)
{
    AssetManager& assets = scene.GetAssetManager();
    /* --- UI --- */
    RE_Atlas *uiAtlas = assets.GetAtlas(AtlasID::UI); AssertNew(uiAtlas);
    RE_Atlas *terrainAtlas = assets.GetAtlas(AtlasID::TERRAIN); AssertNew(terrainAtlas);
    RE_Atlas *enemyAtlas = assets.GetAtlas(AtlasID::ENEMY); AssertNew(enemyAtlas);
    RE_Atlas *leverAtlas = assets.GetAtlas(AtlasID::LEVER); AssertNew(leverAtlas);
    
    /* --- SELECTORS --- */
    std::vector<RE_AtlasPart*> selectorParts;
    std::vector<int> selectorIdxs;
    std::vector<EditorTile::Type> selectorTypes;
    std::vector<bool> reverse;
    /* --- ADD PARTS --- */
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::GROUND); reverse.push_back(false);
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(10); selectorTypes.push_back(EditorTile::Type::STEEP_SLOPE_R); reverse.push_back(false);
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(9); selectorTypes.push_back(EditorTile::Type::STEEP_SLOPE_L); reverse.push_back(false);
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(15); selectorTypes.push_back(EditorTile::Type::GENTLE_SLOPE_R1); reverse.push_back(false);
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(16); selectorTypes.push_back(EditorTile::Type::GENTLE_SLOPE_R2); reverse.push_back(false);
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(12); selectorTypes.push_back(EditorTile::Type::GENTLE_SLOPE_L2); reverse.push_back(false);
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(13); selectorTypes.push_back(EditorTile::Type::GENTLE_SLOPE_L1); reverse.push_back(false);
    
    selectorParts.push_back(terrainAtlas->GetPart("Wood")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::WOOD); reverse.push_back(false);
    selectorParts.push_back(terrainAtlas->GetPart("Brick")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::BRICK); reverse.push_back(false);
    selectorParts.push_back(terrainAtlas->GetPart("Spike")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::SPIKE); reverse.push_back(false);
    selectorParts.push_back(terrainAtlas->GetPart("OneWay")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::ONE_WAY); reverse.push_back(false);
    selectorParts.push_back(enemyAtlas->GetPart("NutIdle")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::FAKE_NUT); reverse.push_back(false);
    selectorParts.push_back(uiAtlas->GetPart("Firefly")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::FAKE_FIREFLY); reverse.push_back(false);
    selectorParts.push_back(leverAtlas->GetPart("LeverOff")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::CHECKPOINT); reverse.push_back(false);
    selectorParts.push_back(terrainAtlas->GetPart("LevelEnd")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::FAKE_FLAG); reverse.push_back(false);
    
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::ROOF); reverse.push_back(true);
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(10); selectorTypes.push_back(EditorTile::Type::STEEP_ROOF_R); reverse.push_back(true);
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(9); selectorTypes.push_back(EditorTile::Type::STEEP_ROOF_L); reverse.push_back(true);
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(15); selectorTypes.push_back(EditorTile::Type::GENTLE_ROOF_R1); reverse.push_back(true);
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(16); selectorTypes.push_back(EditorTile::Type::GENTLE_ROOF_R2); reverse.push_back(true); 
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(12); selectorTypes.push_back(EditorTile::Type::GENTLE_ROOF_L2); reverse.push_back(true);
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(13); selectorTypes.push_back(EditorTile::Type::GENTLE_ROOF_L1); reverse.push_back(true);
    
    selectorParts.push_back(enemyAtlas->GetPart("FlappyIdle")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::FAKE_FLAPPY); reverse.push_back(false);
    selectorParts.push_back(uiAtlas->GetPart("Life")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::SPAWN_POINT); reverse.push_back(false);
    
    float originX = 30.0f;
    float originx = 30.0f;
    float originY = 25.0f;
    for(int i=0; i<selectorParts.size(); i++)
    {
        if(i==15)
        {
            originY += 75.0f; originX = originx; originx=30.0f;
        }
        RE_AtlasPart* part = selectorParts[i];
        int idx = selectorIdxs[i];
        EditorSelectorListener* listener = new EditorUiNS::NsEditorSelectorListener(scene, *this, selectorTypes[i], idx);
        EditorSelector* selector = new EditorSelector(scene, this, part, idx); selector->SetListener(listener);
        selector->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
        selector->GetLocalRect().anchorMax.Set(1.0f, 0.0f);
        selector->GetLocalRect().offsetMin.Set(originx, originY);
        originx += 50.0f;
        selector->GetLocalRect().offsetMax.Set(originx, originY + 50.0f);
        if(reverse[i]) selector->Reverse();
        m_buttonPositions.push_back(selector->GetLocalRect());
        originx += 25.0f;
        m_selectors.push_back(selector);
       
    }
    m_selectors[0]->Select();

    /* --- SAVE BUTTON --- */
    RE_AtlasPart *buttonPart = uiAtlas->GetPart("Button");
    AssertNew(buttonPart);
    SDL_Color colorUp = assets.GetColor(ColorID::NORMAL);
    SDL_Color colorHover = assets.GetColor(ColorID::BLACK);
    SDL_Color colorDown = assets.GetColor(ColorID::NORMAL);
    TTF_Font *font = assets.GetFont(FontID::NORMAL);
    int size {7};
    const std::string texts[7] = {"Sauvegarder", "Nettoyer", "Centrer", "Menu", "Jouer", "<", ">"};
    ButtonListener *listener[7] = { 0 };
    listener[0] = new EditorUiNS::SaveListener(scene, *this);
    listener[1] = new EditorUiNS::ClearListener(scene, *this);
    listener[2] = new EditorUiNS::CenterListener(scene, *this);
    listener[3] = new EditorUiNS::TitleListener(scene, *this);
    listener[4] = new EditorUiNS::PlayListener(scene, *this);
    listener[5] = new EditorUiNS::RollbackListener(scene, *this);
    listener[6] = new EditorUiNS::ForwardListener(scene, *this);

    float bkp = originX;
    for(int i=0; i<size; i++){
        Button *button = new Button(scene, buttonPart);
        button->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
        button->GetLocalRect().anchorMax.Set(1.0f, 0.0f);
        float wrap = (i==5 or i==6) ? 75.0f : 0.0f;
        switch (i)
        { case 0:  originX += 10.0f; break; case 1:  originX += 10.0f; break; case 2:  originX += 10.0f; break; case 3:  originX += 10.0f; break; case 4:  originX += 10.0f; break; case 5:  originX = bkp+10; break; case 6:  originX += 0.0f; break;}
        button->GetLocalRect().offsetMin.Set(originX, 25.0f + wrap);
        switch (i)
        { case 0:  originX += 190.0f; break; case 1:  originX += 140.0f; break; case 2:  originX += 120.0f; break; case 3:  originX += 100.0f; break; case 4:  originX += 110.0f; break; case 5:  originX += 50.0f; break; case 6:  originX += 50.0f; break;}
        button->GetLocalRect().offsetMax.Set(originX, 25.0f + 50.0f + wrap);
        originX += 10.0f;
        m_buttonPositions.push_back(button->GetLocalRect());
        button->SetBorders(new UIBorders(25, 25, 25, 25));
        button->SetListener(listener[i]);

        Text *buttonLabel = new Text(scene, texts[i], font, colorUp);
        button->SetText(buttonLabel, Button::State::UP);

        buttonLabel = new Text(scene, texts[i], font, colorHover);
        button->SetText(buttonLabel, Button::State::HOVER);

        buttonLabel = new Text(scene, texts[i], font, colorDown);
        button->SetText(buttonLabel, Button::State::DOWN);
    }
    
    
}

void EditorUi::DeselectAllSelectors()
{
    for(auto selector : m_selectors)
    {
        selector->Deselect();
    }
}

bool EditorUi::IsOverButtons(float x, float y) const
{
    for(auto button_pos : m_buttonPositions)
    {
        if(x >= button_pos.offsetMin.x && x <= button_pos.offsetMax.x && y >= button_pos.offsetMin.y && y <= button_pos.offsetMax.y)
        {
            return true;
        }
        
    }
    if((y<160.0f and x<710) or y<100.0f) return true;
    return false;
}

