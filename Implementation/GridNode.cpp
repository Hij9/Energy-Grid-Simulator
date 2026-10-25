#include "GridNode.hpp"
#include "CampusNode.hpp"
#include "Vector.hpp"
#include <string>
#include <optional>
#include <limits>

void GridNode::pushToContainer(campusNode *node)
{
    connections.push_back(node);
}

double GridNode::processNetworkTick(size_t tick)
{
    // Reset
    totalNetGridLoad = 0;

    // Loop through the nodes and calculate
    for (campusNode *school : connections)
    {
        school->processTick(tick);

        if (school->getNetLoad() > 0.0)
        {
            totalNetGridLoad += school->getNetLoad() * (1 + lineLossFactor);
        }
        else
        {
            totalNetGridLoad += school->getNetLoad();
        }
    }
    if (totalNetGridLoad > peakThreshold || totalNetGridLoad < backFeedLimit)
    {
        overloadTicks++;
    }
    return totalNetGridLoad;
}

campusNode *GridNode::getCampus(const std::string &name)
{
    for (auto &school : connections)
    {
        if (school->getName() == name)
        {
            return school;
        }
    }
    std::cerr << "Error: School was not found among CampusNodes. Returning nullptr." << std::endl;
    return nullptr;
}

size_t GridNode::getOverloadTicks() const
{
    return overloadTicks;
}

void GridNode::deletePointersInVector()
{
    for (auto &ptr : connections) // You don't see a reference on a pointer every day! (at least for me...)
    {
        delete ptr;
        ptr = nullptr;
    }
}

// Getters

double GridNode::getTotalGridLoad() const
{
    return totalNetGridLoad;
}

Vector<campusNode *> &GridNode::getConnections()
{
    return connections;
}

// Setters
void GridNode::setLineAndThresholds(std::optional<double> lineFactor, std::optional<double> peakThresh, std::optional<double> backFeed)
{
    lineLossFactor = lineFactor.value_or(0.0);
    // If there is no input, make it so that totalNetGridLoad cannot exceed or be under the conditions for an overloaded tick
    peakThreshold = peakThresh.value_or(std::numeric_limits<double>::max());
    backFeedLimit = backFeed.value_or(std::numeric_limits<double>::lowest());
}

void GridNode::setNonDOERetrofits(std::optional<double> demandMult, std::optional<double> solarMult)
{
    for (auto &school : connections)
    {
        if (!school->isDOE())
        {
            school->setMultipliers(demandMult, solarMult);
        }
    }
}