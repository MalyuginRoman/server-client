#include "minor.h"
namespace fs = std::filesystem;

int readFuelConfig()
{
    int dF = 0;
    fs::path current_path = fs::current_path();
    fs::path file_path(current_path);
    for (auto &p : fs::recursive_directory_iterator(file_path.parent_path()))
    {
        std::string current_file = p.path().filename().generic_string();
        std::string readFileName = p.path().parent_path().generic_string() + "/" +
                                   p.path().filename().generic_string();
        if(current_file == "FuelConfige.txt")
        {
            std::fstream readFile(readFileName);
            if (readFile.is_open())
            {
                std::string str;
                readFile >> str;
                dF = stoi(str);
            }
        }
    }
    return dF;
}
