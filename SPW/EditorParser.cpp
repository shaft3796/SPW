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
            switch (m_matrix[x][y])
            {
            case '#':
                editorMap.SetTile(x, y, EditorTile::Type::GROUND, 0);
                break;
            case 'W':
                editorMap.SetTile(x, y, EditorTile::Type::WOOD, 0);
                break;
            case 'A':
                editorMap.SetTile(x, y, EditorTile::Type::SPIKE, 0);
                break;
            case '\\':
                editorMap.SetTile(x, y, EditorTile::Type::STEEP_SLOPE_L, 9);
                break;
            case '/':
                editorMap.SetTile(x, y, EditorTile::Type::STEEP_SLOPE_R, 10);
                break;
            case 'L':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_SLOPE_L2, 12);
                break;
            case 'l':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_SLOPE_L1, 13);
                break;
            case 'r':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_SLOPE_R1, 15);
                break;
            case 'R':
                editorMap.SetTile(x, y, EditorTile::Type::GENTLE_SLOPE_R2, 16);
                break;
            case 'S':
            {
                    editorMap.SetTile(x, y, EditorTile::Type::SPAWN_POINT, 0);
                    scene.SetSpawnSet(true);
                    break;
            }
            case 'F':
            {
                editorMap.SetTile(x, y, EditorTile::Type::FAKE_FLAG, 0);
                break;
            }
            case 'e':
            {
                editorMap.SetTile(x, y, EditorTile::Type::FAKE_NUT, 0);
                break;
            }
            case '=':
            {
                editorMap.SetTile(x, y, EditorTile::Type::ONE_WAY, 0);
                break;
            }
            case 'c':
            {
                editorMap.SetTile(x, y, EditorTile::Type::CHECKPOINT, 0);
                break;
            }
            case 'b':
            {
                    // TODO: BRICK  
                    editorMap.SetTile(x, y, EditorTile::Type::GROUND, 0);
                    break;
            }
            case 'o':
            {
                    editorMap.SetTile(x, y, EditorTile::Type::FAKE_FIREFLY, 0);
                    break;
            }
            default:
                break;
            }
        }
    }
    editorMap.InitTiles();
    
    PE_AABB bounds(0.0f, 0.0f, (float)10, 24.0f * 9.0f / 16.0f);
    Camera* camera = scene.GetActiveCamera();
    camera->SetWorldBounds(bounds);
    
}

