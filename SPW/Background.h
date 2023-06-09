#pragma once

#include "EditorMap.h"
#include "Settings.h"
#include "GameObject.h"

class Background : public GameObject
{
public:
    Background(Scene &scene, Layer layer);
    virtual ~Background();

    virtual void Update() override;
    virtual void Render() override;

    void SetWorldDimensions(const PE_Vec2 &dimensions);
    void SetTexture(int i, SDL_Texture *texture, PE_Vec2 factor);

    inline void SetEditorMap(EditorMap* editorMap) { m_editorMap = editorMap; }

private:
    PE_Vec2 m_worldDim;
    std::vector<SDL_Texture *> m_layers;
    std::vector<PE_Vec2> m_shiftFactors;

    EditorMap* m_editorMap {nullptr};
};

inline void Background::SetWorldDimensions(const PE_Vec2 &dimensions)
{
    m_worldDim = dimensions;
}

inline void Background::SetTexture(int i, SDL_Texture *texture, PE_Vec2 factor)
{
    m_layers[i] = texture;
    m_shiftFactors[i] = factor;
}
