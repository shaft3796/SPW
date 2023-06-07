#pragma once

#include "EditorScene.h"
#include "UIObject.h"

class EditorSelectorListener;

class EditorUi;

class EditorSelector : public UIObject{

public:
    EditorSelector(EditorScene &scene, EditorUi* ui, RE_AtlasPart *atlasPart, int partIdx);
    virtual ~EditorSelector();

    enum class State: int{
        IDLE=0, SELECTED
    };

    void SetListener(EditorSelectorListener *listener);

    void Update() override;
    void Render() override;

    void Deselect();
    void Select();

protected:
    EditorSelectorListener *m_listener;
    RE_AtlasPart *m_atlasPart;
    int m_partIdx;
    EditorUi* m_ui;

    State m_currState;
    State m_prevState;

};

class EditorSelectorListener{
public:
    EditorSelectorListener();
    virtual ~EditorSelectorListener();

    virtual void OnSelect();
    virtual void OnDeselect();
    virtual void OnStateChange(EditorSelector::State state);
    
};

inline void EditorSelector::SetListener(EditorSelectorListener *listener){
    m_listener = listener;
}

inline void EditorSelector::Deselect(){
    m_currState = State::IDLE;
    if(m_listener) m_listener->OnDeselect();
}

inline void EditorSelector::Select(){
    m_currState = State::SELECTED;
    if(m_listener) m_listener->OnSelect();
}