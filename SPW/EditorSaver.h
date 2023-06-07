#pragma once
#include <string>

class EditorMap;
class EditorScene;

class EditorSaver{

public:
    EditorSaver(EditorScene& scene, EditorMap &map);

    void SaveMap(const std::string &path);

private:
    EditorScene &m_scene;
    EditorMap &m_map;
};
