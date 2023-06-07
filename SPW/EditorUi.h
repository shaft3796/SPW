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
    int GetCurrentPartIdx() const;
    void SetCurrentTileType(EditorTile::Type type);
    void SetCurrentPartIdx(int partIdx);

    void DeselectAllSelectors();
    bool IsOverButtons(float x, float y) const;
private:
    EditorTile::Type currentTileType {EditorTile::Type::GROUND};
    int currentPartIdx {0};
    std::vector<EditorSelector*> m_selectors;
    std::vector<UIRect> m_buttonPositions;
};

inline EditorTile::Type EditorUi::GetCurrentTileType() const
{
    return currentTileType;
}

inline void EditorUi::SetCurrentTileType(EditorTile::Type type)
{
    currentTileType = type;
}

inline int EditorUi::GetCurrentPartIdx() const
{
    return currentPartIdx;
}

inline void EditorUi::SetCurrentPartIdx(int partIdx)
{
    currentPartIdx = partIdx;
}