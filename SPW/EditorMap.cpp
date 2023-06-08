#include "EditorMap.h"
#include "Camera.h"
#include "EditorScene.h"
#include "Player.h"


EditorMap::EditorMap(Scene &scene, int width, int height) :
    GameBody(scene, Layer::TERRAIN_BACKGROUND), m_width(width), m_height(height), m_realWidth(1), m_realHeight(1), m_editorScene(scene)
{
    m_name = "EditorMap";

    m_tiles = new EditorTile*[width];
    for (int x = 0; x < width; x++)
    {
        m_tiles[x] = new EditorTile[height];
        for (int y = 0; y < height; y++)
        {
            EditorTile &tile = m_tiles[x][y];
            tile.collider = nullptr;
            tile.partIdx = 0;
            tile.type = EditorTile::Type::EMPTY;
        }
    }

    RE_Atlas *atlas = scene.GetAssetManager().GetAtlas(AtlasID::TERRAIN);
    RE_Atlas *ennemyAtlas = scene.GetAssetManager().GetAtlas(AtlasID::ENEMY);
    RE_Atlas *uiAtlas = scene.GetAssetManager().GetAtlas(AtlasID::UI);
    RE_Atlas *leverAtlas = scene.GetAssetManager().GetAtlas(AtlasID::LEVER);

    m_woodPart = atlas->GetPart("Wood");
    AssertNew(m_woodPart);

    m_oneWayPart = atlas->GetPart("OneWay");
    AssertNew(m_oneWayPart);

    m_terrainPart = atlas->GetPart("Terrain");
    AssertNew(m_terrainPart);

    m_spikePart = atlas->GetPart("Spike");
    AssertNew(m_spikePart);

    //TODO: ADD FAKE TILES HERE
    m_fakePart = atlas->GetPart("Fake");
    AssertNew(m_fakePart);

    m_fakeNutPart = ennemyAtlas->GetPart("NutIdle");
    AssertNew(m_fakeNutPart);

    m_fakeFireflyPart = uiAtlas->GetPart("Firefly");
    AssertNew(m_fakeNutPart);

    m_spawnPointPart = uiAtlas->GetPart("Life");
    AssertNew(m_spawnPointPart);

    m_checkpointPart = leverAtlas->GetPart("LeverOff");
    AssertNew(m_checkpointPart);

    // Couleur des colliders en debug
    m_debugColor.r = 255;
    m_debugColor.g = 200;
    m_debugColor.b = 0;
}

EditorMap::~EditorMap()
{
    if (m_tiles)
    {
        for (int x = 0; x < m_width; x++)
        {
            delete[] m_tiles[x];
        }
        delete[] m_tiles;
    }
}

void EditorMap::SetTile(int x, int y, EditorTile::Type type, int partIdx)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    {
        assert(false);
        // TODO: EXTEND THE MATRIX HERE
    }

    if(x >= m_realWidth)
        m_realWidth = x + 1;
    if(y >= m_realHeight)
        m_realHeight = y + 1;

    EditorTile &tile = m_tiles[x][y];
    Commit commit {x, y, tile.type, type, tile.partIdx, partIdx};
    m_commits.push_back(commit);
    tile.partIdx = partIdx;
    tile.type = type;
}

void EditorMap::InitTiles()
{
    for (int x = 0; x < m_width; x++)
    {
        for (int y = 0; y < m_height; y++)
        {
            EditorTile &tile = m_tiles[x][y];
            EditorTile::Type type = GetTileType(x, y);

            switch (type)
            {
            case EditorTile::Type::GROUND:
                if(GetTileType(x, y + 1) == EditorTile::Type::STEEP_SLOPE_L or GetTileType(x, y + 1) == EditorTile::Type::GENTLE_SLOPE_L1)
                {
                    tile.partIdx = 14;
                }
                else if(GetTileType(x, y + 1) == EditorTile::Type::STEEP_SLOPE_R or GetTileType(x, y + 1) == EditorTile::Type::GENTLE_SLOPE_R1)
                {
                    tile.partIdx = 17;
                }
                else if(GetTileType(x, y + 1) == EditorTile::Type::GENTLE_SLOPE_L2 or GetTileType(x, y + 1) == EditorTile::Type::GENTLE_SLOPE_R2)
                {
                    tile.partIdx = 4;
                }
                else if (IsGround(x, y + 1))
                {
                    tile.partIdx = 4;
                }
                else
                {
                    tile.partIdx = 1;
                }
                break;
            case EditorTile::Type::STEEP_SLOPE_L:
                tile.partIdx = 9;
                break;
            case EditorTile::Type::STEEP_SLOPE_R:
                tile.partIdx = 10;
                break;
            case EditorTile::Type::GENTLE_SLOPE_L1:
                tile.partIdx = 13;
                break;
            case EditorTile::Type::GENTLE_SLOPE_L2:
                tile.partIdx = 12;
                break;
            case EditorTile::Type::GENTLE_SLOPE_R1:
                tile.partIdx = 15;
                break;  
            case EditorTile::Type::GENTLE_SLOPE_R2:
                tile.partIdx = 16;
                break;  

            default:
                tile.partIdx = 0;
                break;
            }
        }
    }
}

void EditorMap::Render()
{
    SDL_Renderer *renderer = m_scene.GetRenderer();
    Camera *camera = m_scene.GetActiveCamera();

    PE_AABB view = camera->GetWorldView();
    int x0 = (int)view.lower.x - 1;
    int y0 = (int)view.lower.y - 1;
    int x1 = (int)view.upper.x + 2;
    int y1 = (int)view.upper.y + 2;

    x0 = PE_Max(x0, 0);
    y0 = PE_Max(y0, 0);
    x1 = PE_Min(x1, m_width);
    y1 = PE_Min(y1, m_height);

    for (int x = x0; x < x1; ++x)
    {
        for (int y = y0; y < y1; ++y)
        {
            EditorTile &tile = m_tiles[x][y];
            PE_Collider *collider = tile.collider;

            PE_Vec2 position((float)x, (float)y);
            SDL_FRect dst = { 0 };

            camera->WorldToView(position, dst.x, dst.y);
            float scale = camera->GetWorldToViewScale();
            dst.w = scale * 1.0f;
            dst.h = scale * 1.0f;

            switch (tile.type)
            {
            case EditorTile::Type::GROUND:
            case EditorTile::Type::STEEP_SLOPE_L:
            case EditorTile::Type::STEEP_SLOPE_R:
            case EditorTile::Type::GENTLE_SLOPE_L1:
            case EditorTile::Type::GENTLE_SLOPE_L2:
            case EditorTile::Type::GENTLE_SLOPE_R1:
            case EditorTile::Type::GENTLE_SLOPE_R2:
                m_terrainPart->RenderCopyF(tile.partIdx, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case EditorTile::Type::FAKE_FLAG:
                m_fakePart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case EditorTile::Type::FAKE_NUT:
                m_fakeNutPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case EditorTile::Type::FAKE_FIREFLY:
                m_fakeFireflyPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case EditorTile::Type::WOOD:
                m_woodPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case EditorTile::Type::ONE_WAY:
                m_oneWayPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case EditorTile::Type::SPIKE:
                m_spikePart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case EditorTile::Type::SPAWN_POINT:
                m_spawnPointPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            case EditorTile::Type::CHECKPOINT:
                m_checkpointPart->RenderCopyF(0, &dst, RE_Anchor::SOUTH_WEST);
                break;
            default:
                break;
            }
        }
    }
}

void EditorMap::Start()
{
}

void EditorMap::OnCollisionStay(GameCollision &collision)
{
}

EditorTile::Type EditorMap::GetTileType(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0)
        return EditorTile::Type::GROUND;
    else if (y >= m_height)
        return EditorTile::Type::EMPTY;
    else
        return m_tiles[x][y].type;
}

bool EditorMap::IsGround(int x, int y) const
{
    switch (GetTileType(x, y))
    {
    case EditorTile::Type::GROUND:
    case EditorTile::Type::STEEP_SLOPE_L:
    case EditorTile::Type::STEEP_SLOPE_R:
    case EditorTile::Type::GENTLE_SLOPE_L1:
    case EditorTile::Type::GENTLE_SLOPE_L2:
    case EditorTile::Type::GENTLE_SLOPE_R1:
    case EditorTile::Type::GENTLE_SLOPE_R2:
        return true;
    default:
        return false;
    }
}


void EditorMap::Rollback(int n)
{
    if(n>(int)m_commits.size())n=(int)m_commits.size();
        
    for(int i=0; i<n; i++)
    {
        Commit &commit = m_commits.back();
        m_tiles[commit.x][commit.y].type = commit.fromType;
        m_tiles[commit.x][commit.y].partIdx = commit.fromPartIdx;
        if(commit.toType == EditorTile::Type::SPAWN_POINT) ((EditorScene&)m_editorScene).SetSpawnSet(false);
        m_commits.pop_back();
    }
}
