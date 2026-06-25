#pragma once
#include <string>
#include "Vector.hpp"

// Contianer for data in the CSV file
struct IntervalData
{
    // Interval Data vars
    std::string Date;
    std::string startTime;
    std::string endTime;
    double Delivered;
    double Received;
};

// Master/School info; container accessed by nodes and abbreviated MMP
struct MasterMeterProfile
{
    int squareFootage = 0;
    bool isDOE = false;
    std::string schoolLevel;
    std::string schoolName;
    Vector<IntervalData> Meter;
};