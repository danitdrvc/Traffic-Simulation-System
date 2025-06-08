#pragma once
#include "Node.h"
#include <vector>

using namespace std;

class Path : public Node
{
private:
    double pathLenght;
    double maxSpeed;
    int maxNumVehicles;
    int currentNumVehicles;
    int scalar;

public:
    Path(string id, double pathLenght, double maxSpeed, int maxNumVehicles, int scalar) : Node(id),
                                                                        pathLenght(pathLenght),
                                                                        maxSpeed(maxSpeed),
                                                                        maxNumVehicles(maxNumVehicles),
                                                                        scalar(scalar)
    {
        currentNumVehicles = 0;
    }
    
    double getPathLenght() const
    {
        return pathLenght;
    }

    double getMaxSpeed() const
    {
        return maxSpeed;
    }

    int getMaxNumVehicles() const
    {
        return maxNumVehicles;
    }  

    string getClassName() const
    {
        return "Path";
    }

    int getCurrentNumVehicles() const
    {
        return currentNumVehicles;
    }

    void setCurrentNumVehicles(int currentNumVehicles)
    {
        this->currentNumVehicles = currentNumVehicles;
    }

    int getScalar() const
    {
        return scalar;
    }

    double calculateSlowdownFactor()
    {
        return scalar * (currentNumVehicles) / maxNumVehicles;
    }

    double calculateAverageSpeed()
    {
        return maxSpeed - calculateSlowdownFactor();
    }
};