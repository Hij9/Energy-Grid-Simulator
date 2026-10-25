// This is the nodes of the star graph behind this simulation, made to be lightweight and represent each school site
#pragma once
#include <string>
#include <optional>
#include "DataTypes.hpp"

class campusNode
{
private:
    double currentNetLoad;
    MasterMeterProfile *profile;

    // For Steady-State simulation

    // Retrofits/Solar Scaling Variables
    double demandMultiplier = 1.0; // represents HVAC, BAS, and Lighting upgrades in percent of delivered
    double solarMultiplier = 1.0;  // represents solar canopy extentions in percent of recieved

    // BESS
    bool BESS_Simulation = false;
    double batteryCapacity = 0.0;
    double batteryCharge = 0.0;
    double maxChargeRate = 0.0; // ?
    double dischargeThreshold = 0.0;

public:
    campusNode();
    campusNode(MasterMeterProfile *assignedProfile);
    double processTick(size_t tick);
    double getNetLoad() const;
    std::string getName() const;
    int getSquareFootage() const;
    bool isDOE() const;
    void setMultipliers(std::optional<double> demandMult = 1.0, std::optional<double> solarMult = 1.0);
    void setBESS_Sim(bool simulationKey,
                     std::optional<double> batCap = std::nullopt,
                     std::optional<double> batCharge = std::nullopt,
                     std::optional<double> chargeRate = std::nullopt,
                     std::optional<double> threshold = std::nullopt);
};