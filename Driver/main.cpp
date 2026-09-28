// Driver program
// NOTE: use g++ -std=c++17 -I Headers Driver/main.cpp Implementation/*.cpp -o test_engine as the command in terminal to update, ./test_engine to run
#include <string>
#include "DataTypes.hpp"
#include "Vector.hpp"
#include "Parser.hpp"
#include "GridNode.hpp"
#include "CampusNode.hpp"

using namespace std;

int main()
{
    Vector<MasterMeterProfile> database = getMMPList("CSV Files");
    GridNode Graph;
    for (auto &m : database)
    {
        Graph.pushToContainer(new campusNode(&m));
    }

    size_t time = database[0].Meter.size();

    for (size_t tick = 0; tick < time; tick++)
    {
        cout << tick << ": Total Load: " << Graph.processNetworkTick(tick) << endl;
    }

    Graph.deletePointersInVector();

    return 0;
}