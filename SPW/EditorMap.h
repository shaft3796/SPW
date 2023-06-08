#pragma once

#include "Settings.h"
#include "GameBody.h"

struct EditorTile
{
    enum class Type : int
    {
        EMPTY, GROUND, WOOD, ONE_WAY, SPIKE, BRICK,
        STEEP_SLOPE_L, STEEP_SLOPE_R,
        GENTLE_SLOPE_L1, GENTLE_SLOPE_L2, GENTLE_SLOPE_R1, GENTLE_SLOPE_R2,
        FAKE_FLAG, FAKE_NUT, FAKE_FIREFLY, SPAWN_POINT, CHECKPOINT, FAKE_FLAPPY
    };
    Type type;
    int partIdx;
    PE_Collider *collider;
};

struct Commit
{
    int x;
    int y;
    EditorTile::Type fromType;
    EditorTile::Type toType;
    int fromPartIdx;
    int toPartIdx;
};

class EditorMap : public GameBody
{
public:
    EditorMap(Scene &scene, int width, int height);
    virtual ~EditorMap();

    void SetTile(int x, int y, EditorTile::Type type, int partIdx, bool extendGroup=false);
    void InitTiles();

    virtual void Update() override;
    virtual void Render() override;
    virtual void Start() override;
    virtual void OnCollisionStay(GameCollision &collision) override;

    int GetRealWidth();
    int GetRealHeight();

    EditorTile::Type GetTileType(int x, int y) const;

    void Forward(int n);
    void ForwardGroup();
    void Rollback(int n);
    void RollbackGroup();

private:

    RE_AtlasPart *m_woodPart;
    RE_AtlasPart *m_oneWayPart;
    RE_AtlasPart *m_terrainPart;
    RE_AtlasPart *m_spikePart;
    RE_AtlasPart *m_fakePart;
    RE_AtlasPart *m_fakeNutPart;
    RE_AtlasPart *m_fakeFireflyPart;
    RE_AtlasPart *m_spawnPointPart;
    RE_AtlasPart *m_checkpointPart;
    RE_AtlasPart *m_fakeFlappyPart;
    RE_AtlasPart *m_brickPart;

    EditorTile **m_tiles;
    int m_width;
    int m_height;
    int m_realWidth;
    int m_realHeight;

    std::vector<Commit> m_commits;
    int m_commit_head = {-1};

    std::vector<int> m_commit_groups;
    int m_group_head = {-1};
    
    bool IsGround(int x, int y) const;

    Scene &m_editorScene;
};

inline void EditorMap::Update()
{
    SetVisible(true);
}

inline int EditorMap::GetRealWidth()
{
    return m_realWidth;
}

inline int EditorMap::GetRealHeight()
{
    return m_realHeight;
}