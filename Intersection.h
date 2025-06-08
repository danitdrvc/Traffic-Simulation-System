#pragma once
#include "Node.h"
#include <vector>
#include <tuple>
#include <memory>

using namespace std;
class Intersection : public Node
{
private:
    vector<tuple<shared_ptr<Node>, shared_ptr<Node>, double, double>> listOfCrossSections; // aggregated Nodes
    int maxNumVehicles;
    int currentNumVehicles;

public:
    Intersection(string id, vector<tuple<shared_ptr<Node>, shared_ptr<Node>, double, double>> listOfCrossSections, int maxNumVehicles) : Node(id),
    listOfCrossSections(listOfCrossSections),
    maxNumVehicles(maxNumVehicles) 
    {
        currentNumVehicles = 0;
    }

    vector<tuple<shared_ptr<Node>, shared_ptr<Node>, double, double>> getListOfCrossSections() const
    {
        return listOfCrossSections;
    }

    int getMaxNumVehicles() const
    {
        return maxNumVehicles;
    }

    string getClassName() const
    {
        return "Intersection";
    }

    int getCurrentNumVehicles() const
    {
        return currentNumVehicles;
    }

    void setCurrentNumVehicles(int currentNumVehicles)
    {
        this->currentNumVehicles = currentNumVehicles;
    }

    double calculateAverageSpeedOnCrossSection(int i)
    {
        if(currentNumVehicles == maxNumVehicles) return 1;
        return get<3>(listOfCrossSections[i]) * (maxNumVehicles - currentNumVehicles) / maxNumVehicles;
    }

    double calcualteAvgSpeedFromNodes(shared_ptr<Node> node1, shared_ptr<Node> node2)
    {
        for (int i = 0; i < listOfCrossSections.size(); i++)
        {
            if (get<0>(listOfCrossSections[i])->getId() == node1->getId() && get<1>(listOfCrossSections[i])->getId() == node2->getId())
            {
                return calculateAverageSpeedOnCrossSection(i);
            }
        }
        return 1;
    }
};