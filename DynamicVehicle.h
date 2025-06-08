#pragma once
#include "Vehicle.h"
#include <algorithm>

class DynamicVehicle : public Vehicle
{
public:
    DynamicVehicle(string start, string stop, double speed, string type) : Vehicle(start, stop, speed, type) {}

    virtual string getClassName() const override
    {
        return "DynamicVehicle";
    }

    void triggerDijkstra(Graph *graph) //poziva se samo kada ce vozilo preci raskrsnicu
    {
        allPassedNodes.erase(allPassedNodes.begin() + getIdxOfCurrentNode() + 1, allPassedNodes.end());
        allTimesToEachNode.erase(allTimesToEachNode.begin() + getIdxOfCurrentNode() + 1, allTimesToEachNode.end());
        allSpeedsOnEachNode.erase(allSpeedsOnEachNode.begin() + getIdxOfCurrentNode() + 1, allSpeedsOnEachNode.end());
        vector<shared_ptr<Node>> passedNodes;
        vector<double> passedTimes;
        vector<double> passedSpeeds;
        passedNodes.insert(passedNodes.end(), allPassedNodes.begin(), allPassedNodes.end());
        passedTimes.insert(passedTimes.end(), allTimesToEachNode.begin(), allTimesToEachNode.end());
        passedSpeeds.insert(passedSpeeds.end(), allSpeedsOnEachNode.begin(), allSpeedsOnEachNode.end());
        
        vector<shared_ptr<Node>> newNodes = this->dijkstra(*graph, getType(), getAllPassedNodes()[getIdxOfCurrentNode()]->getId());
        vector<double> newTimes = this->getAllTimesToEachNode();
        vector<double> newSpeeds = this->getAllSpeedsOnEachNode();

        std::transform(newTimes.begin(), newTimes.end(), newTimes.begin(), [newTimes](double d) { return d - newTimes[0]; });
        newTimes.erase(newTimes.begin());
        std::transform(newTimes.begin(), newTimes.end(), newTimes.begin(), [passedTimes](double d) { return d + passedTimes.back(); });

        allPassedNodes.clear();
        allPassedNodes.insert(allPassedNodes.end(), passedNodes.begin(), passedNodes.end());
        allPassedNodes.insert(allPassedNodes.end(), newNodes.begin()+1, newNodes.end());

        allTimesToEachNode.clear();
        allTimesToEachNode.insert(allTimesToEachNode.end(), passedTimes.begin(), passedTimes.end());
        allTimesToEachNode.insert(allTimesToEachNode.end(), newTimes.begin(), newTimes.end());

        allSpeedsOnEachNode.clear();
        allSpeedsOnEachNode.insert(allSpeedsOnEachNode.end(), passedSpeeds.begin(), passedSpeeds.end());
        allSpeedsOnEachNode.insert(allSpeedsOnEachNode.end(), newSpeeds.begin()+1, newSpeeds.end());
        totalDistance = getTotalDistanceBasedOnSpeedsOnEachNode();
    }
};  