#include <iostream>
#include <string>
#include "CampusNode.hpp"
#include "DataTypes.hpp"

// Default constructor (this is here just in case something goes wrong)
campusNode::campusNode()
{
    profile = nullptr;
    std::cerr << "Warning: instantiation of campusNode without attributing an assigned profile" << std::endl;
}

// Practical constructor, gets the profile
campusNode::campusNode(MasterMeterProfile *assignedProfile)
{
    profile = assignedProfile;
}

// Process (get the data of) a "tick" in simulation
double campusNode::processTick(size_t tick)
{
    currentNetLoad = profile->Meter[tick].Delivered - profile->Meter[tick].Received;
    return currentNetLoad;
}

// Getters

double campusNode::getNetLoad()
{
    return currentNetLoad;
}

std::string campusNode::getName()
{
    return profile->schoolName;
}