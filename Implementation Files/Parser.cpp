
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
    std::ifstream file(path);

    // Error trap
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file" << path << std::endl;
        return Vector<IntervalData>();
    }

    std::string line;
    // Loop through first 12 lines of unneeded data
    for (int i = 0; i < 12; i++)
    {
        std::getline(file, line);
    }

    Vector<IntervalData> parsedTick;

    // Parsing
    while (std::getline(file, line))
    {
        // Data parsing vars (I just want them all to be in one place)
        std::string date;
        std::string start;
        std::string end;
        double delivered;
        double received;

        std::stringstream ss(line);
        std::string cell;

        std::getline(ss, cell, ',');
        date = cell;

        std::getline(ss, cell, ',');
        start = cell;

        std::getline(ss, cell, ',');
        end = cell;

        std::getline(ss, cell, ',');
        delivered = std::stod(cell);

        std::getline(ss, cell, ',');
        received = std::stod(cell);

        IntervalData currentTick;
        currentTick.Date = date;
        currentTick.startTime = start;
        currentTick.endTime = end;
        currentTick.Delivered = delivered;
        currentTick.Received = received;

        parsedTick.push_back(currentTick);
    }

    file.close();

    return parsedTick;
}