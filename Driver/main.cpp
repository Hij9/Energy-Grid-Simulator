// Driver program
// NOTE: use g++ -std=c++17 -I Headers Driver/main.cpp Implementation/*.cpp -o test_engine as the command in terminal to update, ./test_engine to run
#include <string>
#include <fstream>
#include "DataTypes.hpp"
#include "Vector.hpp"
#include "Parser.hpp"
#include "GridNode.hpp"
#include "CampusNode.hpp"
#include "Exporter.hpp"

using namespace std;

int main()
{
    // Simulator Setup
    string simulatorPath = "External Files/Simulator Output/Non-Visualizer Files/SimulatorOutputFile.csv";
    Vector<MasterMeterProfile> database = getMMPList("External Files/CSV Input Files");
    GridNode Graph;
    CSVExporter Output(simulatorPath);

    for (auto &m : database)
    {
        Graph.pushToContainer(new campusNode(&m));
    }
    size_t time = database[0].Meter.size();

    // Simulator config
    campusNode *GHHS = Graph.getCampus("Granite Hills High School");
    if (GHHS != nullptr)
    {
        GHHS->setBESS_Sim(true, 600.0, 0.0, 45.0, 25.0);
    }

    // Simulation
    for (size_t tick = 0; tick < time; tick++)
    {
        Graph.processNetworkTick(tick);
        Output.WriteToFile(tick, database[0].Meter[tick].Date, database[0].Meter[tick].startTime, Graph);
    }
    cout << Graph.getOverloadTicks() << endl;

    // Cleanup
    return 0;
    Graph.deletePointersInVector();
}