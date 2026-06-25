// Driver program
// NOTE: use g++ -std=c++17 -I Headers Driver/main.cpp Implementation/Parser.cpp -o test_engine as a command and then ./test_engine in terminal to run program, otherwise errors will appear
#include <iostream>
#include <string>
#include "DataTypes.hpp"
#include "Vector.hpp"
#include "Parser.hpp"

using namespace std;

int main()
{
    Vector<MasterMeterProfile> database = getMMPList("CSV Files");

    cout << database.size() << endl;

    for (auto &m : database)
    {
        cout << boolalpha << m.isDOE << endl;
        cout << m.schoolLevel << endl;
        cout << m.schoolName << endl;
        cout << m.squareFootage << endl;
        cout << m.Meter.size() << endl;
        cout << endl;
    }
    return 0;
}