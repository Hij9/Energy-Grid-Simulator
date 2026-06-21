
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "Vector.hpp"
#include "DataTypes.hpp"

namespace fs = std::filesystem;

Vector<std::string> getCSVPaths(const std::string &rootFolder)
{
    Vector<std::string> paths;

    for (const auto &entry : fs::recursive_directory_iterator(rootFolder))
    {

        if (!entry.is_regular_file())
        {
            continue;
        }

        if (entry.path().extension() != ".csv")
        {
            continue;
        }

        std::string fullFilePath = entry.path().string();
        paths.push_back(fullFilePath);
    }
    return paths;
}

Vector<IntervalData> parseCSV(const std::string &path)
{
    // Add code here
}