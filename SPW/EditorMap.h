#pragma once

#include "Settings.h"
#include "GameBody.h"

struct EditorTile
{
    enum class Type : int
    {
        EMPTY, GROUND, WOOD, ONE_WAY, SPIKE,
        STEEP_SLOPE_L, STEEP_SLOPE_R,
        GENTLE_SLOPE_L1, GENTLE_SLOPE_L2, GENTLE_SLOPE_R1, GENTLE_SLOPE_R2,
        FAKE_FLAG, FAKE_NUT, FAKE_FIREFLY,
    };
    Type type;
    int partIdx;
    PE_Collider *collider;
};

class EditorMap : public GameBody
{
public:
    EditorMap(Scene &scene, int width, int height);
    virtual ~EditorMap();

    void SetTile(int x, int y, EditorTile::Type type);
    void InitTiles();

    virtual void Update() override;
    virtual void Render() override;
    virtual void Start() override;
    virtual void OnCollisionStay(GameCollision &collision) override;

private:

    RE_AtlasPart *m_woodPart;
    RE_AtlasPart *m_oneWayPart;
    RE_AtlasPart *m_terrainPart;
    RE_AtlasPart *m_spikePart;
    RE_AtlasPart *m_fakePart;
    RE_AtlasPart *m_fakeNutPart;
    RE_AtlasPart *m_fakeFireflyPart;

    EditorTile **m_tiles;
    int m_width;
    int m_height;

    EditorTile::Type GetTileType(int x, int y) const;
    bool IsGround(int x, int y) const;
};

inline void EditorMap::Update()
{
    SetVisible(true);
}
