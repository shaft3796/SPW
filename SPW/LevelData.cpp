#include "LevelData.h"

LevelData::LevelData(const std::string &nameIn, const std::string &pathIn, ThemeID themeIDIn) :
    name(nameIn), path(pathIn), themeID(themeIDIn)
{
}

std::vector<LevelData> LevelData::Init()
{
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
    return data;
}
