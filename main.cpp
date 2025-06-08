#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Graph.h"
#include "Node.h"
#include "Location.h"
#include "Intersection.h"
#include "Path.h"
#include "Vehicle.h"
#include "DynamicVehicle.h"
#include "Simulation.h"

using namespace std;

int main()
{
    Graph graph1;

    ifstream fileInput("input.txt");
    if (fileInput.is_open()) {
        fileInput >> graph1; 
        fileInput.close();
        cout << "Deserialized from file:" << endl << graph1 << endl;
    } else {
        cerr << "Error opening file for deserialization\n";
    }

    Vehicle v1("A", "B", 100, "shortest");
    Vehicle v2("B", "5", 100, "fastest");
    Vehicle v3("A", "B", 60, "shortest");
    Vehicle v4("A", "B", 60, "fastest");
    DynamicVehicle v5("A", "B", 100, "fastest");

    Simulation simulation(&graph1, {&v1, &v2, &v3, &v4, &v5}, 1);

    char userInput;
    while(simulation.isOver() == false)
    {
        cout << "Choose an option:" << endl;
        cout << "  [1] Run simulation step" << endl;
        cout << "  [2] Print current state of vehicles" << endl;
        cout << "  [3] Print average time" << endl;
        cout << "  [4] Check if simulation is over" << endl;
        cin >> userInput;
        if(userInput == '1')
        {
            simulation.runStep();
        }
        else if (userInput == '2')
        {
            simulation.currentStateOfVehicles();
        }
        else if (userInput == '3')
        {
            cout << "Avreage time: " << simulation.averageTime("A", "B") << endl;
        }
        else if (userInput == '4')
        {
            cout << "Simulation is over: " << simulation.isOver() << endl;
        }
    }

    return 0;
}