
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void traverseDirectories(const std::string &targetDirectory)
{
    for (const auto &entry : fs::recursive_directory_iterator(targetDirectory))
    {
        if (entry.is_regular_file())
        {
            continue;
        }

        if (entry.path().extension() != ".csv")
        {
            continue;
        }

        std::string full_path = entry.path().string();

        bool isDOE = false;

        if (full_path.find("DOE Schools") != std::string::npos)
        {
            isDOE = true;
        }
    }
}