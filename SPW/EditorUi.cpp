#include "EditorUi.h"

#include "Button.h"
#include "EditorScene.h"
#include "EditorSelector.h"

class EditorSelector;

namespace EditorUiNS
{
    class NsEditorSelectorListener : public EditorSelectorListener{
    public:
        NsEditorSelectorListener(EditorScene &editorScene, EditorUi &editorUi, EditorTile::Type tileType) : m_editorScene(editorScene), m_editorUi(editorUi), m_tileType(tileType){}
        virtual ~NsEditorSelectorListener(){}

        virtual void OnSelect() override{
            m_editorUi.SetCurrentTileType(m_tileType);
        }

        virtual void OnDeselect() override{
            
        }

    private:
        EditorScene &m_editorScene;
        EditorUi &m_editorUi;
        EditorTile::Type m_tileType;
    };
    
    class QuitListener : public ButtonListener
    {
    public:
        QuitListener(EditorScene &editorScene, EditorUi &editorUi) : m_editorScene(editorScene), m_editorUi(editorUi){}

        virtual void OnPress() override
        {
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

    // Bouton test
    RE_AtlasPart *buttonPart = uiAtlas->GetPart("Button");
    AssertNew(buttonPart);
    SDL_Color colorUp = assets.GetColor(ColorID::NORMAL);
    SDL_Color colorHover = assets.GetColor(ColorID::BLACK);
    SDL_Color colorDown = assets.GetColor(ColorID::NORMAL);
    TTF_Font *font = assets.GetFont(FontID::NORMAL);
    const std::string texts[1] = {"Quitter"};
    ButtonListener *listener[1] = { 0 };
    listener[0] = new EditorUiNS::QuitListener(scene, *this);

    Button *button = new Button(scene, buttonPart);
    button->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
    button->GetLocalRect().anchorMax.Set(1.0f, 0.0f);
    button->GetLocalRect().offsetMin.Set(0.0f, 400);
    button->GetLocalRect().offsetMax.Set(0.0f + 200, 400 + 50);
    button->SetBorders(new UIBorders(25, 25, 25, 25));
    button->SetListener(listener[0]);

    Text *buttonLabel = new Text(scene, texts[0], font, colorUp);
    button->SetText(buttonLabel, Button::State::UP);

    buttonLabel = new Text(scene, texts[0], font, colorHover);
    button->SetText(buttonLabel, Button::State::HOVER);

    buttonLabel = new Text(scene, texts[0], font, colorDown);
    button->SetText(buttonLabel, Button::State::DOWN);

    // SELECTOR TEST
    std::vector<RE_AtlasPart*> selectorParts;
    std::vector<int> selectorIdxs;
    std::vector<EditorTile::Type> selectorTypes;
    /* --- ADD PARTS --- */
    selectorParts.push_back(terrainAtlas->GetPart("Terrain")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::GROUND);
    selectorParts.push_back(terrainAtlas->GetPart("Wood")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::WOOD);
    selectorParts.push_back(terrainAtlas->GetPart("Spike")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::SPIKE);
    selectorParts.push_back(terrainAtlas->GetPart("OneWay")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::ONE_WAY);
    selectorParts.push_back(enemyAtlas->GetPart("NutIdle")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::FAKE_NUT);
    selectorParts.push_back(uiAtlas->GetPart("Firefly")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::FAKE_FIREFLY);
    selectorParts.push_back(terrainAtlas->GetPart("LevelEnd")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::FAKE_FLAG);
    selectorParts.push_back(uiAtlas->GetPart("Life")); selectorIdxs.push_back(0); selectorTypes.push_back(EditorTile::Type::SPAWN_POINT);
    
    float originX = 30.0f;
    for(int i=0; i<selectorParts.size(); i++)
    {
        RE_AtlasPart* part = selectorParts[i];
        int idx = selectorIdxs[i];
        EditorSelectorListener* listener = new EditorUiNS::NsEditorSelectorListener(scene, *this, selectorTypes[i]);
        EditorSelector* selector = new EditorSelector(scene, this, part, idx); selector->SetListener(listener);
        selector->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
        selector->GetLocalRect().anchorMax.Set(1.0f, 0.0f);
        selector->GetLocalRect().offsetMin.Set(originX, 25.0f);
        originX += 50.0f;
        selector->GetLocalRect().offsetMax.Set(originX, 25.0f + 50.0f);
        originX += 25.0f;
        m_selectors.push_back(selector);
    }
    m_selectors[0]->Select();
    
}

void EditorUi::DeselectAllSelectors()
{
    for(auto selector : m_selectors)
    {
        selector->Deselect();
    }
}

