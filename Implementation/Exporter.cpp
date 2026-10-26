#include "Exporter.hpp"
#include "Vector.hpp"
#include "GridNode.hpp"
#include <fstream>
#include <string>
#include <iostream>

// Just in case; catch incorrect usage
CSVExporter::CSVExporter()
{
    std::cerr << "Error: no file path given, cannot open file." << std::endl;
    exit(1);
}

CSVExporter::CSVExporter(std::string relativePath)
{
    outFile.open(relativePath);
    if (!outFile.is_open())
    {
        std::cerr << "Error: Output File Unable to Open." << std::endl;
        exit(1); // Nothing else can happen in the future without this step so exit program here
    }
}

void CSVExporter::WriteToFile(size_t tick, std::string date, std::string time, GridNode &grid)
{
    outFile << tick << "," << date << "," << time << "," << grid.getTotalGridLoad() << "," << "PUSD_Grid" << std::endl;
    for (auto &i : grid.getConnections())
    {
        outFile << tick << "," << date << "," << time << "," << i->getNetLoad() << "," << i->getName() << "," << i->getSquareFootage() << std::endl;
    }
}

void CSVExporter::closeFile()
{
    outFile.close();
}

CSVExporter::~CSVExporter()
{
    outFile.close();
}