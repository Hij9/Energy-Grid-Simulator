#pragma once
#include <string>
#include "Vector.hpp"

/**
 * @brief Container for the data in a CSV file
 *
 * @details Represents all attributes/columns of data in a single line of a CSV file. This is the lowest level of data management
 * that isn't the CSV file itself. The struct is only constructed and written to during parsing, though its attributes are used in many
 * places throughout the program.
 */
struct IntervalData
{
    std::string Date;
    std::string startTime;
    std::string endTime;
    double Delivered;
    double Received;
};

// Master/School info; container accessed by nodes and abbreviated MMP
/**
 * @brief Container of all CSV information (IntervalData) belonging to a campus/site
 *
 * @details Is the second lowest level of data management, and holds everything to know about the attributes of a school site.
 * Whenever information about a school is needed, one can expect this container and its attributes to be used somewhere.
 * For multi-metered campuses, parallel CSV files are summed during parsing so Meter always represents a unified chronological
 * timeline.
 */
struct MasterMeterProfile
{
    int squareFootage = 0;
    bool isDOE = false;
    std::string schoolLevel;
    std::string schoolName;
    Vector<IntervalData> Meter;
};

/**
 * @brief
 *
 */
struct WeatherData
{
    double temperature; // IN CELCIUS
    double irradiance;
    std::string timestamp; // Debugging purposes
};