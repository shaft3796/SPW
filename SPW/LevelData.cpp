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
    initLevel("../Assets/Level/Le Tronc.txt");
    initLevel("../Assets/Level/Lile.txt");
    initLevel("../Assets/Level/LevelDemo.txt");
    initLevel("../Assets/Level/Bac à sable (Ciel).txt");
    initLevel("../Assets/Level/Bac à sable (Montagnes).txt");
    initLevel("../Assets/Level/Bac à sable (Lac).txt");
    
    std::vector<LevelData> data;

    data.push_back(LevelData(
        "Démo",
        u8"../Assets/Level/LevelDemo.txt",
        ThemeID::SKY
    ));
    data.push_back(LevelData(
        "L'Ile'",
        u8"../Assets/Level/Lile.txt",
        ThemeID::SKY
    ));
    data.push_back(LevelData(
        u8"Le Tronc",
        u8"../Assets/Level/Le Tronc.txt",
        ThemeID::MOUNTAINS
    ));
    data.push_back(LevelData(
        "Bac à sable (Ciel)",
        "../Assets/Level/Bac à sable (Ciel).txt",
        ThemeID::SKY
    ));
    data.push_back(LevelData(
        "Bac à sable (Montagnes)",
        "../Assets/Level/Bac à sable (Montagnes).txt",
        ThemeID::MOUNTAINS
    ));
    data.push_back(LevelData(
        "Bac à sable (Lac)",
        "../Assets/Level/Bac à sable (Lac).txt",
        ThemeID::LAKE
    ));
        
    
    return data;
}
