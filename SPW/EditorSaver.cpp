#include "EditorSaver.h"

#include "EditorMap.h"
#include "fstream"
#include "Settings.h"

EditorSaver::EditorSaver(EditorScene& scene, EditorMap& map) : m_scene(scene), m_map(map)
{
}

void EditorSaver::SaveMap(const std::string& path)
{
    std::fstream fs;
    fs.open(path, std::fstream::out);
    
    for(int y=m_map.GetRealWidth(); y>=0; y--)
    {
            for(int x=0; x<m_map.GetRealWidth(); x++)
        {
            EditorTile::Type type = m_map.GetTileType(x, y);
            switch(type)
            {
                //TODO: HANDLE SLOPES
                case EditorTile::Type::EMPTY:
                    fs << ".";
                    break;
                case EditorTile::Type::GROUND:
                    fs << "#";
                    break;
                case EditorTile::Type::STEEP_SLOPE_L:
                    fs << "\\";
                    break;
                case EditorTile::Type::STEEP_SLOPE_R:
                    fs << "/";
                    break;
                case EditorTile::Type::GENTLE_SLOPE_L2:
                    fs << "L";
                    break;
                case EditorTile::Type::GENTLE_SLOPE_L1:
                    fs << "l";
                    break;
                case EditorTile::Type::GENTLE_SLOPE_R1:
                    fs << "r";
                    break;
                case EditorTile::Type::GENTLE_SLOPE_R2:
                    fs << "R";
                    break;
                case EditorTile::Type::WOOD:
                    fs << "W";
                    break;
                case EditorTile::Type::ONE_WAY:
                    fs << "=";
                    break;
                case EditorTile::Type::SPIKE:
                    fs << "A";
                    break;
                case EditorTile::Type::FAKE_FLAG:
                    fs << "F";
                    break;
                case EditorTile::Type::FAKE_NUT:
                    fs << "e";
                    break;
                case EditorTile::Type::FAKE_FIREFLY:
                    fs << "o";
                    break;
                case EditorTile::Type::SPAWN_POINT:
                    fs << "S";
                    break;
                case EditorTile::Type::CHECKPOINT:
                    fs << "c";
                    break;
                
                default:
                    fs << ".";
                    printf("WARNING IN EDITOR SAVER LINE, UNANDLED TYPE: %d\n", type);
            }
        }
        fs << std::endl;
    }
    printf("SAVE FINISHED\n");
}
