#pragma once
#include <string>
#include "Vector.hpp"
#include "DataTypes.hpp"
#include <unordered_map>

/**
 * @brief Given a directory, it finds all of the CSV files within that directory
 *
 * @param rootFolder The initial directory that marks the start of the search
 * @return An array of all of the paths of the CSV files found
 */
Vector<std::string> getCSVPaths(const std::string &rootFolder);

/**
 * @brief Takes a CSV file and extracts its data. Assumes a certain format for each CSV file
 *
 * @param path The path of the CSV file
 * @return Formats each row into IntervalData ( @see DataTypes.hpp), Vector for storage
 */
Vector<IntervalData> parseCSV(const std::string &path);

/**
 * @brief Combines and organizes getCSVPaths and parseCSV to place data into each campus/site.
 *
 * @param rootFolder @see getCSVPaths(), this is used for the exact same purpose
 * @return An array of each site with all of its data
 */
Vector<MasterMeterProfile> getMMPList(const std::string &rootFolder);

/**
 * @brief
 *
 * @param weatherPath
 * @return Vector<WeatherData>
 */
Vector<WeatherData> parseWeatherCSV(const std::string &weatherPath);

/**
 * @brief
 *
 * @param weatherPath
 * @return Vector<std::string>
 */
Vector<std::string> getStartAndEndDates(const std::string &weatherPath);

/**
 * @brief A dictionary via hashmap of all site squarefootage, needs to be updated manually
 */
std::unordered_map<std::string, int> sqft();