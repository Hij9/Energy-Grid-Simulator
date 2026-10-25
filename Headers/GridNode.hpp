// This is the "star" in the star graph structure. It will connect and link all of the nodes together to form a simulation of PUSD (aka this is the most important part)
#pragma once
#include <optional>
#include <limits>
#include "Vector.hpp"
#include "CampusNode.hpp"

class GridNode
{
private:
    Vector<campusNode *> connections; // edge container
    double totalNetGridLoad = 0.0;

    // Grid Line Losses/Transformer Stress
    double lineLossFactor = 0.0; // Default 0.02? This value should always be in percent 0 < LLF < 1
    size_t overloadTicks = 0;
    double peakThreshold = std::numeric_limits<double>::max();
    double backFeedLimit = std::numeric_limits<double>::lowest();

public:
    void pushToContainer(campusNode *node);
    double processNetworkTick(size_t tick);
    double getTotalGridLoad() const;
    size_t getOverloadTicks() const;
    Vector<campusNode *> &getConnections();
    void deletePointersInVector();
    campusNode *getCampus(const std::string &name);
    // When returning to add more aggregate numbers based on simulation requirements, add more getters here
    // Setters
    void setLineAndThresholds(std::optional<double> lineFactor = std::nullopt,
                              std::optional<double> peakThresh = std::nullopt,
                              std::optional<double> backFeed = std::nullopt);
    void setNonDOERetrofits(std::optional<double> demandMult = 1.0, std::optional<double> solarMult = 1.0);
};