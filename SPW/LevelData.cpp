#include "LevelData.h"
#include <fstream>

void initLevel(std::string path)
{
    std::fstream fs; fs.open(path, std::fstream::out);
    // If the file is empty, we fill it
    std::fstream rb; rb.open(path, std::fstream::in);
    if (rb.peek() == std::fstream::traits_type::eof())
    {
        rb.close();
        fs << ".S.\n###\n";
    }
}

LevelData::LevelData(const std::string &nameIn, const std::string &pathIn, ThemeID themeIDIn) :
    name(nameIn), path(pathIn), themeID(themeIDIn)
{
}

std::vector<LevelData> LevelData::Init()
{
    initLevel("../Assets/Level/Sky01.txt");
    initLevel("../Assets/Level/Sky02.txt");
    initLevel("../Assets/Level/Mountains01.txt");
    initLevel("../Assets/Level/Mountains02.txt");
    initLevel("../Assets/Level/Lake01.txt");
    initLevel("../Assets/Level/Lake02.txt");
    
    std::vector<LevelData> data;

    data.push_back(LevelData(
        u8"Montagnes",
        u8"../Assets/Level/Level01.txt",
        ThemeID::MOUNTAINS
    ));
    data.push_back(LevelData(
        u8"Démo",
        u8"../Assets/Level/LevelDemo.txt",
        ThemeID::SKY
    ));
    data.push_back(LevelData(
        u8"Sky01",
        u8"../Assets/Level/Sky01.txt",
        ThemeID::SKY
    ));
    data.push_back(LevelData(
        u8"Sky02",
        u8"../Assets/Level/Sky02.txt",
        ThemeID::SKY
    ));
    data.push_back(LevelData(
        u8"Montagnes01",
        u8"../Assets/Level/Mountains01.txt",
        ThemeID::MOUNTAINS
    ));
    data.push_back(LevelData(
        u8"Montagnes02",
        u8"../Assets/Level/Mountains02.txt",
        ThemeID::MOUNTAINS
    ));
    data.push_back(LevelData(
        u8"Lac01",
        u8"../Assets/Level/Lake01.txt",
        ThemeID::LAKE
    ));
    data.push_back(LevelData(
        u8"Lac02",
        u8"../Assets/Level/Lake02.txt",
        ThemeID::LAKE
    ));
    
    return data;
}
