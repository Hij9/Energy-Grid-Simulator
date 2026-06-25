// CSV Parser Header File

#pragma once
#include <string>
#include "Vector.hpp"
#include "DataTypes.hpp"
#include <unordered_map>

Vector<std::string> getCSVPaths(const std::string &rootFolder);

Vector<IntervalData> parseCSV(const std::string &path);

Vector<MasterMeterProfile> getMMPList(const std::string &rootFolder);

std::unordered_map<std::string, int> sqft();