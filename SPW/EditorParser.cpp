#include "EditorParser.h"
#include "EditorMap.h"
#include "EditorScene.h"
#include "Brick.h"
#include "Checkpoint.h"
#include "Nut.h"
#include "LevelEnd.h"
#include "Bonus.h"
#include "Camera.h"
#include "Firefly.h"
#include "Oneway.h"

EditorParser::EditorParser(const std::string& path)
{
    FILE *levelFile = fopen(path.c_str(), "rb");
    AssertNew(levelFile);

    bool isValidChar[128] = { 0 };
    isValidChar['\n'] = true;
    for (int i = '!'; i < '~'; i++)
    {
        isValidChar[i] = true;
    }

    fseek(levelFile, 0, SEEK_END);
    long fileSize = ftell(levelFile);
    rewind(levelFile);

    char *buffer = new char[fileSize];
    fread(buffer, 1, fileSize, levelFile);
    fclose(levelFile);
    levelFile = NULL;

    int height = 0;
    int width = 0;
    int i;
    for (i = 0; i < fileSize; i++)
    {
        char c = buffer[i];
        if (isValidChar[(int)c])
        {
            if (c == '\n')
            {
                height = 1;
                break;
            }
            else
            {
                width++;
            }
        }
        else
        {
            printf("INFO - Invalid char (%c)\n", c);
        }
    }
    if (height == 0)
    {
        printf("ERROR - Incorrect level file\n");
        assert(false);
        abort();
    }
    int w = 0;
    for (i = i + 1; i < fileSize; ++i)
    {
        char c = buffer[i];
        if (isValidChar[(int)c])
        {
            if (c == '\n')
            {
                if (w == 0)
                {
                    break;
                }
                height++;

                if (w != width)
                {
                    printf("ERROR - Incorrect level file\n");
                    assert(false);
                    abort();
                }
                w = 0;
            }
            else
            {
                w++;
            }
        }
        else
        {
            printf("INFO - Invalid char (%c)\n", c);
        }
    }

    // Initialisation du parser
    m_width = width;
    m_height = height;

    m_matrix = new char *[m_width];
    for (int x = 0; x < width; ++x)
    {
        m_matrix[x] = new char[height];
    }

    // Remplissage de la matrice du niveau
    int x = 0;
    int y = height - 1;
    for (i = 0; i < fileSize; ++i)
    {
        char c = buffer[i];
        if (isValidChar[(int)c])
        {
            if (c == '\n')
            {
                x = 0;
                y--;
                if (y < 0)
                {
                    break;
                }
            }
            else
            {
                m_matrix[x][y] = c;
                x++;
            }
        }
    }

    delete[] buffer;
}

EditorParser::~EditorParser()
{
    for (int x = 0; x < m_width; ++x)
    {
        delete[] m_matrix[x];
    }
    delete[] m_matrix;
}

void EditorParser::InitScene(EditorScene& scene, EditorMap& editorMap) const
{
    for (int x = 0; x < m_width; ++x)
    {
        for (int y = 0; y < m_height; ++y)
        {
            PE_Vec2 position((float)x, (float)y);
            bool extend = (x!=0 || y!=0);
            switch (m_matrix[x][y])
            {
            case '#':
                editorMap.SetTile(x, y, EditorTile::Type::GROUND, 0, extend);
                break;
            case 'W':
                editorMap.SetTile(x, y, EditorTile::Type::WOOD, 0, extend);
                break;
            case 'A':
                editorMap.SetTile(x, y, EditorTile::Type::SPIKE, 0, extend);
                break;
            case '\\':
                editorMap.SetTile(x, y, EditorTile::Type::STEEP_SLOPE_L, 9, extend);
                break;
            case '/':
                editorMap.SetTile(x, y, EditorTile::Type::STEEP_SLOPE_R, 10, extend);
                break;
            case 'L':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_SLOPE_L2, 12, extend);
                break;
            case 'l':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_SLOPE_L1, 13, extend);
                break;
            case 'r':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_SLOPE_R1, 15, extend);
                break;
            case 'R':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_SLOPE_R2, 16, extend);
                break;
            case '*':
                editorMap.SetTile(x, y, EditorTile::Type::ROOF, 0, extend);
                break;
            case ']':
                editorMap.SetTile(x, y, EditorTile::Type::STEEP_ROOF_L, 9, extend);
                break;
            case '[':
                editorMap.SetTile(x, y, EditorTile::Type::STEEP_ROOF_R, 10, extend);
                break;
            case '2':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_ROOF_L2, 12, extend);
                break;
            case '1':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_ROOF_L1, 13, extend);
                break;
            case '3':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_ROOF_R1, 15, extend);
                break;
            case '4':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_ROOF_R2, 16, extend);
                break;
            case 'S':
            {
                    editorMap.SetTile(x, y, EditorTile::Type::SPAWN_POINT, 0, extend);
                    scene.SetSpawnSet(true);
                    break;
            }
            case 'F':
            {
                editorMap.SetTile(x, y, EditorTile::Type::FAKE_FLAG, 0, extend);
                break;
            }
            case 'e':
            {
                editorMap.SetTile(x, y, EditorTile::Type::FAKE_NUT, 0, extend);
                break;
            }
            case '=':
            {
                editorMap.SetTile(x, y, EditorTile::Type::ONE_WAY, 0, extend);
                break;
            }
            case 'c':
            {
                editorMap.SetTile(x, y, EditorTile::Type::CHECKPOINT, 0, extend);
                break;
            }
            case 'b':
            {
                    editorMap.SetTile(x, y, EditorTile::Type::BRICK, 0, extend);
                    break;
            }
            case 'f':
            {
                    editorMap.SetTile(x, y, EditorTile::Type::FAKE_FLAPPY, 0, extend);
                    break;
            }
            case 'o':
            {
                    editorMap.SetTile(x, y, EditorTile::Type::FAKE_FIREFLY, 0, extend);
                    break;
            }
            default:
                printf("WARNING, UNHANDLED TILE.\n");
                if(not extend) editorMap.SetTile(x, y, EditorTile::Type::EMPTY, 0, extend);
                break;
                break;
            }
        }
    }
    editorMap.InitTiles();
    
    PE_AABB bounds(0.0f, 0.0f, (float)10, 24.0f * 9.0f / 16.0f);
    Camera* camera = scene.GetActiveCamera();
    camera->SetWorldBounds(bounds);
    
}

