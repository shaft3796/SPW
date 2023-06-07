#pragma once

#include <complex.h>

#include "Settings.h"

class EditorScene;

class EditorParser
{
public:
    EditorParser(const std::string &path);
    ~EditorParser();

    void InitScene(EditorScene &scene) const;

private:
    char**m_matrix;
    int m_width;
    int m_height;
};