#pragma once

#include "UIObject.h"

class EditorScene;

class EditorCanvas : public UIObject{
public:
    EditorCanvas(EditorScene &scene);

    void Update() override;
    void OnRespawn() override;
    void Render() override;

private:
    EditorScene &m_editorScene;
};