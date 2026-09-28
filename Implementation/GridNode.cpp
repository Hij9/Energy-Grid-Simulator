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
