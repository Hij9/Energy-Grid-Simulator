#include <iostream>
#include <string>
#include <algorithm>
#include <optional>
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
    currentNetLoad = profile->Meter[tick].Delivered * demandMultiplier - profile->Meter[tick].Received * solarMultiplier;
    // BESS Simulation phase
    if (BESS_Simulation == false)
    {
        return currentNetLoad;
    }
    if (currentNetLoad < 0.0) // Looking at charging the battery
    {
        double batteryCharging = std::min({-currentNetLoad, batteryCapacity - batteryCharge, maxChargeRate}); // Find how much energy to put into the battery

        // Add to the battery and take away from the currentNetLoad
        batteryCharge += batteryCharging;
        currentNetLoad += batteryCharging;
    }
    else if (currentNetLoad > dischargeThreshold) // Discharging the battery
    {
        double batteryDispensing = std::min({currentNetLoad - dischargeThreshold, batteryCharge, maxChargeRate}); // Find how much energy to discharge from the

        // Take away from the battery and reduce the currentNetLoad
        batteryCharge -= batteryDispensing;
        currentNetLoad -= batteryDispensing;
    }
    return currentNetLoad;
}

// Getters, Setters & Misc.

double campusNode::getNetLoad() const
{
    return currentNetLoad;
}

std::string campusNode::getName() const
{
    return profile->schoolName;
}

int campusNode::getSquareFootage() const
{
    return profile->squareFootage;
}

bool campusNode::isDOE() const
{
    return profile->isDOE;
}

void campusNode::setMultipliers(std::optional<double> demandMult, std::optional<double> solarMult)
{
    demandMultiplier = demandMult.value_or(1.0);
    solarMultiplier = solarMult.value_or(1.0);
}

// Configure the BESS Simulation
void campusNode::setBESS_Sim(bool simulationKey, std::optional<double> batCap, std::optional<double> batCharge, std::optional<double> chargeRate, std::optional<double> threshold)
{
    BESS_Simulation = simulationKey;
    batteryCapacity = batCap.value_or(0.0);
    batteryCharge = batCharge.value_or(0.0);
    maxChargeRate = chargeRate.value_or(0.0);
    dischargeThreshold = threshold.value_or(0.0);

    // To protect against user error
    if (batteryCapacity <= 0.0 || batteryCharge < 0.0 || maxChargeRate <= 0.0 || dischargeThreshold < 0.0)
    {
        BESS_Simulation = false;
        std::cout << "BESS Simulation unable to run; failure to set proper parameters." << std::endl;
    }
}