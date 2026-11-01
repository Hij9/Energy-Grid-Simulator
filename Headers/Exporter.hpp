#pragma once
#include <string>
#include <fstream>
#include "GridNode.hpp"
#include "Vector.hpp"

/**
 * @brief Handles outputting data into a file so python can take over for the frontend
 */
class CSVExporter
{
private:
    std::ofstream outFile;

public:
    /**
     * @brief Disable use of a default constructor to ensure no write-to-file errors
     */
    CSVExporter() = delete;

    /**
     * @brief Construct a new CSVExporter object with route to a file
     *
     * @param relativePath Path to the output file
     */
    CSVExporter(std::string relativePath);

    /**
     * @brief Function that writes to the output file
     *
     * @param tick Chronological 15-minute index (0 to ~38,111) in the MasterMeterProfile timeline
     * @param date The day, month, and year of the tick
     * @param time The start time (formatted hh:mm AM/PM) of the tick
     * @param grid Access to the star node so the function can access the edges and the total grid load (kWh)
     */
    void WriteToFile(size_t tick, std::string date, std::string time, GridNode &grid);

    /**
     * @brief Closes the outFile object
     * @details File closure is needed before the object goes out of scope to prevent C++ holding on to the
     * file while Python tries to read, so this function is called.
     */
    void closeFile();

    /**
     * @brief Destroy the CSVExporter object, closing the file
     * @details It is kept here just in case it may be needed in the future
     */
    ~CSVExporter();
};