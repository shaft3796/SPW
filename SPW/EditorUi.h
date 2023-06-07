#pragma once

#include "UIObject.h"
#include "EditorCanvas.h"
#include "EditorMap.h"

class EditorScene;
class EditorSelector;

class EditorUi : public UIObject
{
public:
    EditorUi(EditorScene &scene);

    EditorTile::Type GetCurrentTileType() const;
    void SetCurrentTileType(EditorTile::Type type);

    void DeselectAllSelectors();
private:
    EditorTile::Type currentTileType {EditorTile::Type::GROUND};
    std::vector<EditorSelector*> m_selectors;
};

inline EditorTile::Type EditorUi::GetCurrentTileType() const
{
    return currentTileType;
}

inline void EditorUi::SetCurrentTileType(EditorTile::Type type)
{
    currentTileType = type;
}