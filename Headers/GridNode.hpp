#pragma once
#include <optional>
#include <limits>
#include "Vector.hpp"
#include "CampusNode.hpp"

/**
 * @brief The "star/hub" node in the graph. Also the highest (grid) level of data management.
 *
 * @details Links all campusNodes; simulation is accessed through use of the processNetworkTick() method
 * and a loop. There should only be one of these nodes instantiated throughout the entire simulation.
 */
class GridNode
{
private:
    Vector<campusNode *> connections; // edge container
    double totalNetGridLoad = 0.0;

    // Grid Line Losses/Transformer Stress
    double lineLossFactor = 0.0; // Represents the loss of electricity due to resistance in wires in the grid (should be thought of as a percent)
    size_t overloadTicks = 0;
    double peakThreshold = std::numeric_limits<double>::max();    // Represents the transformer ceiling/maximum capacity
    double backFeedLimit = std::numeric_limits<double>::lowest(); // Represents the transformer's solar energy generation absorption limit, causing reverse power flow

public:
    /**
     * @brief Establishes the edge to a campusNode
     *
     * @param node Represents a campusNode to be added to the graph
     */
    void pushToContainer(campusNode *node);

    /**
     * @brief Loops through each site/campusNode and calls its processTick() method, modifying results as simulation needs
     *
     * @param tick Chronological 15-minute index (0 to ~38,111) in the MasterMeterProfile timeline
     * @return double Returns totalNetGridLoad (summed campusNode's netGridLoad across all sites)
     */
    double processNetworkTick(size_t tick);

    /**
     * @brief Returns totalNetGridLoad
     */
    double getTotalGridLoad() const;

    /**
     * @brief Returns OverloadTicks
     *
     * @return Counts how many ticks are above or below the threshold or backfeed limit
     */
    size_t getOverloadTicks() const;

    /**
     * @brief Provide outside access to the edges
     */
    Vector<campusNode *> &getConnections();

    /**
     * @brief Cleanup function to delete all of the campusNode objects
     */
    void deletePointersInVector();

    /**
     * @brief Returns a specific campusNode searched via name
     *
     * @param name Name of site to search for
     * @return Pointer to found (or nullptr for not found) site
     */
    campusNode *getCampus(const std::string &name);

    // Setters

    /**
     * @brief Set simulation requirements for the grid line losses/transformer stress
     *
     * @param lineFactor Input for setting lineLossFactor value
     * @param peakThresh Input for setting peakThreshold value
     * @param backFeed Input for setting backFeedLimit value
     */
    void setLineAndThresholds(std::optional<double> lineFactor = std::nullopt,
                              std::optional<double> peakThresh = std::nullopt,
                              std::optional<double> backFeed = std::nullopt);

    /**
     * @brief Set all Non-DOE school's modifiers to simulate DOE grant work on these schools
     *
     * @param demandMult Change the delivered (kWh) to replicate HVAC, LED, and BAS upgrades
     * @param solarMult Change the received (kWh) to replicate solar upgrades
     */
    void setNonDOERetrofits(std::optional<double> demandMult = 1.0, std::optional<double> solarMult = 1.0);
};