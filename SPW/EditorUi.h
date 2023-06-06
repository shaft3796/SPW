#pragma once

#include "UIObject.h"
#include "EditorCanvas.h"

class EditorScene;

class EditorUi : public UIObject
{
public:
    EditorUi(EditorScene &scene);
};