#pragma once
#include <string>
#include <optional>
#include "DataTypes.hpp"

/**
 * @brief This is the class for the leaves of the graph.
 * @details represents the second highest level of data management (school site-wide); simulation manipulations/data processing happens here.
 */
class campusNode
{
private:
    double currentNetLoad;
    MasterMeterProfile *profile;

    // For Steady-State simulation

    // Retrofits/Solar Scaling Variables
    double demandMultiplier = 1.0; // represents HVAC, BAS, and Lighting upgrades in percent of delivered
    double solarMultiplier = 1.0;  // represents solar canopy extensions in percent of received

    // BESS
    bool BESS_Simulation = false;
    double batteryCapacity = 0.0;
    double batteryCharge = 0.0;
    double maxChargeRate = 0.0;
    double dischargeThreshold = 0.0;

public:
    /**
     * @brief Disable use of default constructor to prevent empty node errors
     */
    campusNode() = delete;

    /**
     * @brief Construct a new campus Node object
     *
     * @param assignedProfile Represents the site's Master Meter Profile (where its CSV data is)
     */
    campusNode(MasterMeterProfile *assignedProfile);

    /**
     * @brief Where simulation processes happen
     *
     * @param tick Chronological 15-minute index (0 to ~38,111) in the MasterMeterProfile timeline
     * @return double Returns currentNetLoad
     * @details Net load is in kWh per 15-minute interval, where positive (> 0) = net grid consumption and negative (< 0) = net solar export back to the grid
     */
    double processTick(size_t tick);

    /**
     * @brief Return currentNetLoad  @see campusNode::processTick(size_t tick)
     */
    double getNetLoad() const;

    /**
     * @brief Return site name
     */
    std::string getName() const;

    /**
     * @brief Return square footage of the site
     */
    int getSquareFootage() const;

    /**
     * @brief Checks if the campus is part of the 7 DOE schools
     *
     * @return true This school was part of the CAPS-DOE grant
     * @return false This school was not part of the CAPS-DOE grant
     */
    bool isDOE() const;

    /**
     * @brief Set the delivered/received multipliers to imitate retrofits (going to change in the future)
     *
     * @param demandMult Delivered Multiplier
     * @param solarMult Received Multiplier
     */
    void setMultipliers(std::optional<double> demandMult = 1.0, std::optional<double> solarMult = 1.0);

    /**
     * @brief Set up the BESS Simulator with proper values
     *
     * @param simulationKey Simulation only starts when this is true
     * @param batCap Capacity of battery (in kWh)
     * @param batCharge Current battery charge (in kWh)
     * @param chargeRate Rate of charge of battery (in kWh per tick)
     * @param threshold Peak-shaving demand threshold (kWh/tick) above which the battery discharges
     */
    void setBESS_Sim(bool simulationKey,
                     std::optional<double> batCap = std::nullopt,
                     std::optional<double> batCharge = std::nullopt,
                     std::optional<double> chargeRate = std::nullopt,
                     std::optional<double> threshold = std::nullopt);
};