// This is the "star" in the star graph structure. It will connect and link all of the nodes together to form a simulation of PUSD (aka this is the most important part)
#pragma once
#include "Vector.hpp"
#include "CampusNode.hpp"

class GridNode
{
private:
    Vector<campusNode *> connections; // edge container
    double totalNetGridLoad = 0.0;

public:
    void pushToContainer(campusNode *node);
    double processNetworkTick(size_t tick);
    double getTotalGridLoad();
    // When returning to add more aggregate numbers based on simulation requirements, add more getters here
};