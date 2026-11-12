#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
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
        else if (entry.path().extension() != ".csv")
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
        std::cerr << "Error: could not open file " << path << std::endl;
        return Vector<IntervalData>();
    }

    std::string line;

    // Loop through first 12 lines of unneeded data
    for (int i = 0; i < 12; i++)
    {
        std::getline(file, line);
    }

    Vector<IntervalData> parsedTick;
    IntervalData currentTick;
    std::string cell;
    std::stringstream ss;

    // Parsing loop (per line of data)
    while (std::getline(file, line))
    {
        // Clean the string so it doesn't crash the program because of stod
        line.erase(std::remove(line.begin(), line.end(), '"'), line.end());

        // Gaurd clause to ensure the line has 4 commas (rest is garbage data) so stod doesn't throw a fit (again)
        if (std::count(line.begin(), line.end(), ',') != 4)
        {
            continue;
        }

        ss.clear();
        ss.str(line);

        // Parse and store
        std::getline(ss, currentTick.Date, ',');

        std::getline(ss, currentTick.startTime, ',');

        std::getline(ss, currentTick.endTime, ',');

        std::getline(ss, cell, ',');
        currentTick.Delivered = std::stod(cell);

        std::getline(ss, cell, ',');
        currentTick.Received = std::stod(cell);

        parsedTick.push_back(currentTick);
    }

    file.close();

    return parsedTick;
}

Vector<MasterMeterProfile> getMMPList(const std::string &rootFolder)
{
    Vector<std::string> CSVP = getCSVPaths(rootFolder);

    Vector<MasterMeterProfile> SchoolInformation;

    std::unordered_map<std::string, int> sqftInfo = sqft();

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
            // Note 2 (from the future): I'm not sure if I will.
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

            auto find = sqftInfo.find(name);
            if (find != sqftInfo.end())
            {
                Campus.squareFootage = find->second;
            }
            else
            {
                std::cerr << "Error: Square Footage not found for school " << name << std::endl;
            }

            SchoolInformation.push_back(Campus);
            continue;
        }

        // If the object does exist

        Vector<IntervalData> singleMeter = parseCSV(filePath);

        // Check for missing lines in meter data
        if (address->Meter.size() != singleMeter.size())
        {
            std::cerr << "Error: Meter Data Misalignment at object " << address->schoolName << std::endl;
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

Vector<WeatherData> parseWeatherCSV(const std::string &weatherPath)
{
    std::ifstream weather(weatherPath);

    if (!weather.is_open())
    {
        // I might go back and take all of these types of lines out and replace with exceptions, I just don't really like nesting in a try-catch block
        std::cerr << "Error: Weather CSV file failed to open." << std::endl;
        return Vector<WeatherData>();
    }

    std::string line;
    std::string data;
    WeatherData currentLine;
    Vector<WeatherData> output;
    std::stringstream ss;

    while (std::getline(weather, line))
    {
        // I'm unsure if I have to face the same issue in this CSV but I'll clean the line just in case
        line.erase(std::remove(line.begin(), line.end(), '"'), line.end());

        if (std::count(line.begin(), line.end(), ',') != 2)
        {
            continue;
        }

        ss.clear();
        ss.str(line);

        std::getline(ss, currentLine.timestamp, ',');

        std::getline(ss, data, ',');
        currentLine.temperature = std::stod(data);

        std::getline(ss, data, ',');
        currentLine.irradiance = std::stod(data);

        output.push_back(currentLine);
    }
    weather.close();
    return output;
}

Vector<std::string> getStartAndEndDates(const std::string &weatherPath)
{
    // Open file and check
    std::ifstream weather(weatherPath, std::ios::binary);

    if (!weather.is_open())
    {
        std::cerr << "Error: Weather CSV file failed to open." << std::endl;
        return Vector<std::string>();
    }

    // Vars
    std::string line;
    std::string firstLine;
    std::string lastLine;
    Vector<std::string> output;
    bool isQuoted = false;

    while (std::getline(weather, line))
    {
        // Check for correct comma count (2)
        if (std::count(line.begin(), line.end(), ',') != 2)
        {
            continue;
        }

        // Check for and and erase quotes on the first line
        if (std::count(line.begin(), line.end(), '"') > 0)
        {
            isQuoted = true;
            line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
        }

        // Get the first line and put it to the output
        firstLine = line.substr(0, line.find(','));
        output.push_back(firstLine);
        break;
    }

    // Move to end of file and save position in pos
    weather.seekg(0, std::ios::end);
    std::streampos pos = weather.tellg();
    char throwAway;

    while (pos > 0)
    {
        pos -= 1;
        weather.seekg(pos);
        weather.get(throwAway);
        if (throwAway != '\n' && throwAway != '\r')
        {
            break;
        }
    }

    while (pos > 0)
    {
        pos -= 1;
        weather.seekg(pos);
        weather.get(throwAway);
        if (throwAway == '\n')
        {
            break;
        }
    }

    // Make sure the quotes don't interfere with data collection
    if (isQuoted)
    {
        pos += 1;
        weather.seekg(pos);
        std::getline(weather, lastLine, '"');
    }
    else
    {
        std::getline(weather, lastLine, ',');
    }

    output.push_back(lastLine);

    // Format the output into SCE | (0,4) = YYYY, (5,2) = MM, (8,2) = DD | + extra space at the end
    output[0] = output[0].substr(5, 2) + "/" + output[0].substr(8, 2) + "/" + output[0].substr(0, 4) + " ";
    output[1] = output[1].substr(5, 2) + "/" + output[1].substr(8, 2) + "/" + output[1].substr(0, 4) + " ";

    return output;
}

std::unordered_map<std::string, int> sqft()
{
    std::unordered_map<std::string, int> campusSize;
    // DOE Elementaries
    campusSize["Belleview Elementary"] = 31539;
    campusSize["John J. Doyle Elementary"] = 46508;
    campusSize["Los Robles Elementary"] = 44876;
    campusSize["Roche Elementary"] = 25177;
    campusSize["West Putnam Elementary"] = 37286;

    // DOE Middle/High Schools
    campusSize["Granite Hills High School"] = 149387;
    campusSize["Bartlett Middle School"] = 64458;

    // Non-DOE Elementaries
    campusSize["Monte Vista Elementary"] = 45259;
    campusSize["Olive Street Elementary"] = 52911;
    campusSize["Santa Fe Elementary"] = 47043;
    campusSize["Vandalia Elementary"] = 53791;
    campusSize["Westfield Elementary"] = 47494;

    // Non-DOE Middle Schools
    campusSize["Pioneer Middle School"] = 60306;
    campusSize["Sequoia Middle School"] = 51692;

    // Non-DOE High Schools
    campusSize["Citrus High School"] = 12668;
    campusSize["Harmony Magnet Academy"] = 51392;
    campusSize["Monache High School"] = 196387;
    campusSize["Porterville High School"] = 258834;
    campusSize["Strathmore High School"] = 64012;
    /* Could not include Porterville Military Academy and Butterfield because of a lack of sqft knowledge for the former and a lack of electric data on the latter.
    A reason is that these two schoools are conjoined in the same building. Include note about not adding in Preschools later */
    return campusSize;
}