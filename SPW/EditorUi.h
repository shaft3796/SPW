#pragma once

#include "UIObject.h"
#include "EditorCanvas.h"
#include "StaticMap.h"

class EditorScene;
class EditorSelector;

class EditorUi : public UIObject
{
public:
    EditorUi(EditorScene &scene);

    Tile::Type GetCurrentTileType() const;
    void SetCurrentTileType(Tile::Type type);

    void DeselectAllSelectors();
private:
    Tile::Type currentTileType {Tile::Type::GROUND};
    std::vector<EditorSelector*> m_selectors;
};

inline Tile::Type EditorUi::GetCurrentTileType() const
{
    return currentTileType;
}

inline void EditorUi::SetCurrentTileType(Tile::Type type)
{
    currentTileType = type;
}