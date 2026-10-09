#pragma once

#include <string>
#include <fstream>
#include "GridNode.hpp"
#include "Vector.hpp"

// Class to handle outputting data into a file so python can take over for the frontend, stores a "variable" to a file
class CSVExporter
{
private:
    std::ofstream outFile;

public:
    CSVExporter();
    CSVExporter(std::string relativePath); // Actual constructor
    void WriteToFile(size_t tick, std::string date, std::string time, GridNode &grid);
    ~CSVExporter();
};