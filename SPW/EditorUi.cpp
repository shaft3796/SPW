#include "EditorUi.h"

#include "Button.h"
#include "EditorScene.h"

namespace EditorUiNS
{
    /*class SelectionListener : public ButtonListener
    {
    public:
        SelectionListener(TitleScene &titleScene, StartScreen &startScreen) :
            m_titleScene(titleScene), m_startScreen(startScreen)
        {
        }
        virtual void OnPress() override
        {
            LevelSelection *levelSelection = new LevelSelection(m_titleScene);
            levelSelection->SetParent(m_startScreen.GetParent());
            m_startScreen.Delete();
        }
    private:
        TitleScene &m_titleScene;
        StartScreen &m_startScreen;
    };*/
    class QuitListener : public ButtonListener
    {
    public:
        QuitListener(EditorScene &editorScene, EditorUi &editorUi) : m_editorScene(editorScene), m_editorUi(editorUi){}

        virtual void OnPress() override
        {
            printf("TRIGGERED\n");
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
    RE_Atlas *uiAtlas = assets.GetAtlas(AtlasID::UI);
    AssertNew(uiAtlas);

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
}
