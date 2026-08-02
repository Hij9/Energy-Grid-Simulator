// This is the nodes of the star graph behind this simulation, made to be lightweight and represent each school site
#pragma once
#include <string>
#include "DataTypes.hpp"

class campusNode
{
private:
    double currentNetLoad;
    MasterMeterProfile *profile;

public:
    campusNode();
    campusNode(MasterMeterProfile *assignedProfile);
    double processTick(size_t tick);
    double getNetLoad();
    std::string getName();
};