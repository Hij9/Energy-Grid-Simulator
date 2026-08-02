#include "GridNode.hpp"
#include "CampusNode.hpp"
#include "Vector.hpp"
#include <string>

void GridNode::pushToContainer(campusNode *node)
{
    connections.push_back(node);
}

double GridNode::processNetworkTick(size_t tick)
{
    // Reset all aggregate state variables to zero
    totalNetGridLoad = 0;

    // Loop through the nodes and do what's needed for each aggregate state variable
    for (campusNode *school : connections)
    {
        // totalNetGridLoad:
        school->processTick(tick);
        totalNetGridLoad += school->getNetLoad();
    }
    return totalNetGridLoad;
}

// Getters

double GridNode::getTotalGridLoad()
{
    return totalNetGridLoad;
}