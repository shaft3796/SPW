#pragma once

#include "UIObject.h"
#include "Text.h"

class EditorScene;

class EditorHeader : public UIObject{
public:
    EditorHeader(EditorScene &scene);

    void Update() override;

private:
    EditorScene &m_editorScene;
    // TODO: Text *m_fireflyCount ?
};