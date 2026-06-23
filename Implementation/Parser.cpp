
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Vector.hpp"
#include "DataTypes.hpp"
#include "Parser.hpp"

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
        // Clean the string so it doesn't crash the program because of stod
        line.erase(std::remove(line.begin(), line.end(), '"'), line.end());

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

Vector<MasterMeterProfile> getMMPList(const std::string &rootFolder)
{
    Vector<std::string> CSVP = getCSVPaths(rootFolder);

    Vector<MasterMeterProfile> SchoolInformation;

    for (std::string &filePath : CSVP)
    {
        // To place in MMP (see DataTypes.hpp)
        std::string name;
        std::string level;
        bool DOE = false;

        // Flag for not creating a duplicate object
        bool found = false;

        // Is school DOE?
        if (filePath.find("DOE Schools") != std::string::npos)
        {
            DOE = true;
        }

        // What level is school?
        if (filePath.find("Elementary") != std::string::npos)
        {
            level = "Elementary School";
        }
        else if (filePath.find("High School") != std::string::npos)
        {
            level = "High School";
        }
        else if (filePath.find("Middle School") != std::string::npos)
        {
            level = "Middle School";
        }
        else
        {
            // Error trap. I want this program to work perfectly when in use, so if it doesn't have the level, the function detonates
            std::cerr << "Error: school level not found." << std::endl;
            return Vector<MasterMeterProfile>();
            // Note: make sure to add a check somewhere in the future to see if this is empty before simulation start. If it is, then terminate program
        }

        size_t endpos = filePath.find_last_of('/');
        size_t beginpos = filePath.find_last_of('/', endpos - 1);

        name = filePath.substr(beginpos + 1, endpos - beginpos - 1);

        // Check to see if an object with the same school name already exists in the master vector/container and store memory address if true
        MasterMeterProfile *address = nullptr;
        for (auto &object : SchoolInformation)
        {
            if (object.schoolName == name)
            {
                found = true;
                address = &object;
                break;
            }
        }

        // Guard clause if the object doesn't already exist
        if (found != true)
        {
            MasterMeterProfile Campus;
            Campus.Meter = parseCSV(filePath);
            Campus.schoolLevel = level;
            Campus.schoolName = name;
            Campus.isDOE = DOE;
            SchoolInformation.push_back(Campus);
            continue;
        }

        // If the object does exist

        Vector<IntervalData> singleMeter = parseCSV(filePath);

        // Check for missing lines in meter data
        if (address->Meter.size() != singleMeter.size())
        {
            std::cerr << "Error: Meter Data Misalignment at " << address->schoolName << std::endl;
            continue;
        }

        for (size_t i = 0; i < singleMeter.size(); i++)
        {
            address->Meter[i].Delivered += singleMeter[i].Delivered;
            address->Meter[i].Received += singleMeter[i].Received;
        }
    }

    return SchoolInformation;
}