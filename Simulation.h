#pragma once
#include "Vehicle.h"
#include "DynamicVehicle.h"
#include "Graph.h"
#include <iomanip>

using namespace std;

class Simulation
{
private:
    Graph *graph;
    vector<Vehicle *> listOfVehicles;
    double timeStep;

public:
    Simulation(Graph *graph, vector<Vehicle *> listOfVehicles, double timeStep) : graph(graph),
                                                                               listOfVehicles(listOfVehicles),
                                                                               timeStep(timeStep)
    {
        for (int i = 0; i < listOfVehicles.size(); i++)
        {
            if(listOfVehicles[i]->getType() == "shortest")
            {
                //listOfVehicles[i]->shortestPath(*graph);
                listOfVehicles[i]->dijkstra(*graph, "shortest", listOfVehicles[i]->getStart());
            }
            else if(listOfVehicles[i]->getType()  == "fastest")
            {
                //listOfVehicles[i]->fastestPath(*graph);
                listOfVehicles[i]->dijkstra(*graph, "fastest", listOfVehicles[i]->getStart());
            }
        }
    }

    void runStep()
    {
        // pretpostavljamo da je timeStep dovoljno malen tako da vozila ne mogu preci vise od jednog cvora u jednom koraku
        for (int i = 0; i < listOfVehicles.size(); i++)
        {
            Vehicle *currentVehicle = listOfVehicles[i];
            // ako je vozilo doslo do kraja
            if (currentVehicle->getIdxOfCurrentNode() >= currentVehicle->getAllPassedNodes().size() - 1)
            {
                continue;
            }
            // ako vozilo da dalje vozi na istom cvoru
            if (currentVehicle->getAllTimesToEachNode()[currentVehicle->getIdxOfCurrentNode()] >=
                currentVehicle->getCurrentTime() + timeStep)
            {
                currentVehicle->setCurrentTime(currentVehicle->getCurrentTime() + timeStep);
                currentVehicle->setDistancePassed(currentVehicle->getDistancePassed() + currentVehicle->getAllSpeedsOnEachNode()[currentVehicle->getIdxOfCurrentNode()] * timeStep);
            }
            // ako ce vozilo preci na sljedeci cvor
            else
            {
                shared_ptr<Node> currentNode = currentVehicle->getAllPassedNodes()[currentVehicle->getIdxOfCurrentNode()];
                shared_ptr<Node> nextNode = currentVehicle->getAllPassedNodes()[currentVehicle->getIdxOfCurrentNode()+1];
                // ako sljedeci cvor nije pun -> vozilo se pomjera na sljedeci cvor
                if (nextNode->getClassName() == "Path" && dynamic_cast<Path *>(nextNode.get())->getCurrentNumVehicles() < dynamic_cast<Path *>(nextNode.get())->getMaxNumVehicles())
                {
                    if(currentNode->getClassName() == "Path")
                    {
                        dynamic_cast<Path *>(currentNode.get())->setCurrentNumVehicles(dynamic_cast<Path *>(currentNode.get())->getCurrentNumVehicles() - 1);
                    }
                    else if(currentNode->getClassName() == "Intersection")
                    {
                        dynamic_cast<Intersection *>(currentNode.get())->setCurrentNumVehicles(dynamic_cast<Intersection *>(currentNode.get())->getCurrentNumVehicles() - 1);
                    }
                    double timeToNextNode = currentVehicle->getAllTimesToEachNode()[currentVehicle->getIdxOfCurrentNode()] - currentVehicle->getCurrentTime();
                    double timeOnTheNextNode = timeStep - timeToNextNode;
                    currentVehicle->setDistancePassed(currentVehicle->getDistancePassed() + currentVehicle->getAllSpeedsOnEachNode()[currentVehicle->getIdxOfCurrentNode()] * timeToNextNode + currentVehicle->getAllSpeedsOnEachNode()[currentVehicle->getIdxOfCurrentNode()+1] * timeOnTheNextNode);
                    currentVehicle->setCurrentTime(currentVehicle->getCurrentTime() + timeStep);
                    currentVehicle->setIdxOfCurrentNode(currentVehicle->getIdxOfCurrentNode() + 1);
                    dynamic_cast<Path *>(nextNode.get())->setCurrentNumVehicles(dynamic_cast<Path *>(nextNode.get())->getCurrentNumVehicles() + 1);
                }
                else if (nextNode->getClassName() == "Intersection" && dynamic_cast<Intersection *>(nextNode.get())->getCurrentNumVehicles() <= dynamic_cast<Intersection *>(nextNode.get())->getMaxNumVehicles())
                {
                    if(currentNode->getClassName() == "Path")
                    {
                        dynamic_cast<Path *>(currentNode.get())->setCurrentNumVehicles(dynamic_cast<Path *>(currentNode.get())->getCurrentNumVehicles() - 1);
                        if(currentVehicle->getClassName() == "DynamicVehicle")
                        {
                            dynamic_cast<DynamicVehicle *>(currentVehicle)->triggerDijkstra(graph);
                        }
                    }
                    else if(currentNode->getClassName() == "Intersection")
                    {
                        dynamic_cast<Intersection *>(currentNode.get())->setCurrentNumVehicles(dynamic_cast<Intersection *>(currentNode.get())->getCurrentNumVehicles() - 1);
                    }
                    double timeToNextNode = currentVehicle->getAllTimesToEachNode()[currentVehicle->getIdxOfCurrentNode()] - currentVehicle->getCurrentTime();
                    double timeOnTheNextNode = timeStep - timeToNextNode;
                    currentVehicle->setDistancePassed(currentVehicle->getDistancePassed() + currentVehicle->getAllSpeedsOnEachNode()[currentVehicle->getIdxOfCurrentNode()] * timeToNextNode + currentVehicle->getAllSpeedsOnEachNode()[currentVehicle->getIdxOfCurrentNode()+1] * timeOnTheNextNode);
                    currentVehicle->setCurrentTime(currentVehicle->getCurrentTime() + timeStep);
                    currentVehicle->setIdxOfCurrentNode(currentVehicle->getIdxOfCurrentNode() + 1);
                    dynamic_cast<Intersection *>(nextNode.get())->setCurrentNumVehicles(dynamic_cast<Intersection *>(nextNode.get())->getCurrentNumVehicles() + 1);
                }
                else if(nextNode->getClassName() == "Location")
                {
                    if(currentNode->getClassName() == "Path")
                    {
                        dynamic_cast<Path *>(currentNode.get())->setCurrentNumVehicles(dynamic_cast<Path *>(currentNode.get())->getCurrentNumVehicles() - 1);
                    }
                    else if(currentNode->getClassName() == "Intersection")
                    {
                        dynamic_cast<Intersection *>(currentNode.get())->setCurrentNumVehicles(dynamic_cast<Intersection *>(currentNode.get())->getCurrentNumVehicles() - 1);
                    }
                    currentVehicle->setCurrentTime(currentVehicle->getCurrentTime() + timeStep);
                    currentVehicle->setIdxOfCurrentNode(currentVehicle->getIdxOfCurrentNode() + 1);
                }

                // ako je sljececi cvor pun
                else
                {
                    currentVehicle->setDistancePassed(currentVehicle->getDistancePassed() + currentVehicle->getAllSpeedsOnEachNode()[currentVehicle->getIdxOfCurrentNode()] * (currentVehicle->getAllTimesToEachNode()[currentVehicle->getIdxOfCurrentNode()] - currentVehicle->getCurrentTime()));
                    currentVehicle->setCurrentTime(currentVehicle->getAllTimesToEachNode()[currentVehicle->getIdxOfCurrentNode()]);
                }
            }
            /*
            - sva vozila krecu sa pocetne lokacije
            - provjerava se na kojem je cvoru na osnovu currentTime+timeStep i vremena koje je potrebno da se predje taj cvor(.getAllTimesToEachNode)
                - ako je na istom i dalje, currentTime vozila += timeStep
                - ako bi trebao preci na sljedeci cvor, provjerava se da li je taj cvor pun
                    - ako jeste - vozilo ostaje na istom cvoru currentTime vozila = vremenu koje je potrebno da se predje taj cvor
                    - ako nije - vozilo se pomjera na sljedeci cvor - currentTime vozila += timeStep && idxOfCurrentNode++
            */
        }
    }

    bool isOver()
    {
        bool isOver = true;
        for (int i = 0; i < listOfVehicles.size(); i++)
        {
            Vehicle *currentVehicle = listOfVehicles[i];
            if (currentVehicle->getStop() != currentVehicle->getAllPassedNodes()[currentVehicle->getIdxOfCurrentNode()]->getId()) // TODO testirati
            {
                isOver = false;
                break;
            }
        }
        if (isOver)
        {
            cout << "Simulation is over!" << endl;
        }
        return isOver;
    }

    void currentStateOfVehicles()
    {
        cout << "Current state of vehicles:" << endl << endl;
        for (int i = 0; i < listOfVehicles.size(); i++)
        {
            Vehicle *currentVehicle = listOfVehicles[i];
            cout << "Vehicle " << i + 1 << " (" << currentVehicle->getStart() << " -> " << currentVehicle->getStop() << ", " << currentVehicle->getSpeed() << " m/s" << ")" << endl;
            if (currentVehicle->getIdxOfCurrentNode() != currentVehicle->getAllPassedNodes().size() - 1)
            {
                cout << "Going through nodes:     ";
                cout << std::setw(2) << left << currentVehicle->getAllPassedNodes()[0]->getId();
                for(int i=1; i<currentVehicle->getAllPassedNodes().size() ; i++)
                {
                    cout<< " -> " << std::setw(2) << left << currentVehicle->getAllPassedNodes()[i]->getId();
                }
                cout << endl;
                cout << "Vehicle position:        " << setw(currentVehicle->getIdxOfCurrentNode()*6) << " " << "^" << endl;
                cout << "Time spent moving:       " << setw(currentVehicle->getIdxOfCurrentNode()*6) << " " << currentVehicle->getCurrentTime() << endl;
                cout << "Distance to end:         " << setw(currentVehicle->getIdxOfCurrentNode()*6) << " " << currentVehicle->getTotalDistance() - currentVehicle->getDistancePassed() << endl << endl << endl;
            }
            else
            {
                cout << endl << endl << "Vehicle finished jurney" << endl << endl << endl << endl;
            }
        }
        cout << endl << endl;
    }

    double averageTime(string idStart, string idStop)
    {
        double sumTime = 0;
        int numVehicles = 0;
        for(int i=0; i<listOfVehicles.size(); i++)
        {
            Vehicle* currentVehicle = listOfVehicles[i];
            if(currentVehicle->getStart() == idStart && currentVehicle->getStop() == idStop)
            {
                sumTime += currentVehicle->getAllTimesToEachNode().back();
                numVehicles++;
            }
        }
        if(numVehicles == 0) return 0;
        return sumTime / numVehicles;
    }

};