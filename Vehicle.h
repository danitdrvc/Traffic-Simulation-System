#pragma once
#include <vector>
#include <limits.h>
#include <iostream>
#include <algorithm>

#include "Node.h"
#include "Graph.h"

using namespace std;

class Vehicle
{
protected:
    string start;
    string stop;
    double speed;
    string type;

    // varijable koje sluze za cuvanje podataka tokom simulacije ili koje tek dobijaju vrijednost nakon poziva funkcije shortestPath ili fastestPath
    vector<shared_ptr<Node>> allPassedNodes;
    vector<double> allTimesToEachNode;
    vector<double> allSpeedsOnEachNode;
    double currentTime;
    int idxOfCurrentNode;
    double distancePassed;
    double totalDistance;

public:
    Vehicle(string start, string stop, double speed, string type) : start(start), stop(stop), speed(speed), type(type)
    {
        currentTime = 0;
        idxOfCurrentNode = 0;
        distancePassed = 0;
        totalDistance = 0;
    }

    virtual string getClassName() const
    {
        return "Vehicle";
    }

    string getStart() const
    {
        return start;
    }

    string getStop() const
    {
        return stop;
    }

    double getSpeed() const
    {
        return speed;
    }

    string getType() const
    {
        return type;
    }

    int getIdxOfCurrentNode() const
    {
        return idxOfCurrentNode;
    }

    void setIdxOfCurrentNode(int idxOfCurrentNode)
    {
        this->idxOfCurrentNode = idxOfCurrentNode;
    }

    double getCurrentTime() const
    {
        return currentTime;
    }

    void setCurrentTime(double currentTime)
    {
        this->currentTime = currentTime;
    }

    vector<shared_ptr<Node>> getAllPassedNodes() const
    {
        return allPassedNodes;
    }

    void setAllPassedNodes(vector<shared_ptr<Node>> allPassedNodes)
    {
        this->allPassedNodes = allPassedNodes;
    }

    vector<double> getAllTimesToEachNode() const
    {
        return allTimesToEachNode;
    }

    vector<double> getAllSpeedsOnEachNode() const
    {
        return allSpeedsOnEachNode;
    }

    double getTotalDistance() const
    {
        return totalDistance;
    }

    double getDistancePassed() const
    {
        return distancePassed;
    }

    void setDistancePassed(double distancePassed)
    {
        this->distancePassed = distancePassed;
    }


    // Pomocne funkcije-------------------------------------------------------------------------------------------------
protected:
    double getTotalDistanceBasedOnSpeedsOnEachNode()
    {
        double totalDistance = allSpeedsOnEachNode[0] * allTimesToEachNode[0];
        for(int i=0; i<allTimesToEachNode.size()-1; i++)
        {
            totalDistance += allSpeedsOnEachNode[i+1] * (allTimesToEachNode[i+1] - allTimesToEachNode[i]);
        }
        return totalDistance;
    }

private:
    int minValueIndex(vector<double> dist, vector<bool> sptSet)
    {
        int min = INT_MAX;
        int min_index;
    
        for (int v = 0; v < dist.size(); v++)
            if (sptSet[v] == false && dist[v] <= min)
                min = dist[v], min_index = v;
    
        return min_index;
    }
    
    vector<shared_ptr<Node>> getShortestPathNodes(int start, int end, vector<tuple<int,int>> prev, Graph& graph)
    {
        vector<shared_ptr<Node>> shortestPath;
        int u = end;

        if (get<0>(prev[u]) != -1 || u == start) {
            while (u != -1) {
                if(graph.getNodes()[u]->getClassName() == "Path" && get<1>(prev[u]) != -1 && graph.getNodes()[get<0>(prev[u])]->getClassName() == "Intersection")
                {
                    shortestPath.push_back(graph.getNodes()[u]);
                    shortestPath.push_back(graph.getNodes()[get<0>(prev[u])]);
                    u = get<1>(prev[u]);
                }
                else{
                    shortestPath.push_back(graph.getNodes()[u]);
                    u = get<0>(prev[u]);
                }
            }
            reverse(shortestPath.begin(), shortestPath.end());
        }

        return shortestPath;
    }

    vector<double> getTimesToEachNode(int start, int end, vector<tuple<int,int>> prev, vector<double> times, Graph& graph)
    {
        vector<double> allTimesToEachNode;
        int u = end;

        if (get<0>(prev[u]) != -1 || u == start) {
            while (get<0>(prev[u]) != -1) {
                if(graph.getNodes()[u]->getClassName() == "Path" && get<1>(prev[u]) != -1 && graph.getNodes()[get<0>(prev[u])]->getClassName() == "Intersection")
                {
                    allTimesToEachNode.push_back(times[u]);
                    allTimesToEachNode.push_back(times[get<1>(prev[u])] + dynamic_cast<Path *>(graph.getNodes()[get<1>(prev[u])].get())->getPathLenght() / min(speed, dynamic_cast<Path *>(graph.getNodes()[u].get())->calculateAverageSpeed()));
                    u = get<1>(prev[u]);
                }
                else{
                    allTimesToEachNode.push_back(times[u]);
                    u = get<0>(prev[u]);
                }
            }
            reverse(allTimesToEachNode.begin(), allTimesToEachNode.end());
        }

        return allTimesToEachNode;
    }

    vector<double> getTimesToEachNodeFromDist(int start, int end, vector<tuple<int,int>> prev, vector<double> dist, Graph& graph)
    {
        vector<double> allDistToEachNode;
        vector<double> allTimesToEachNode;
        int u = end;

        // provjeriti da li se cvor moze dostici iz zadatog cvora
        if (get<0>(prev[u]) != -1 || u == start) {
            while (get<0>(prev[u]) != -1) {
                if(graph.getNodes()[u]->getClassName() == "Path" && get<1>(prev[u]) != -1 && graph.getNodes()[get<0>(prev[u])]->getClassName() == "Intersection")
                {
                    allDistToEachNode.push_back(dist[u]);
                    allDistToEachNode.push_back(dist[get<1>(prev[u])] + dynamic_cast<Path *>(graph.getNodes()[get<1>(prev[u])].get())->getPathLenght());
                    u = get<1>(prev[u]);
                }
                else{
                    allDistToEachNode.push_back(dist[u]);
                    u = get<0>(prev[u]);
                }
            }
            reverse(allDistToEachNode.begin(), allDistToEachNode.end());
        }

        allTimesToEachNode.push_back(allDistToEachNode[0] / allSpeedsOnEachNode[1]);
        for(int i=0; i<allDistToEachNode.size()-1; i++)
        {
            allTimesToEachNode.push_back(allTimesToEachNode[i] + (allDistToEachNode[i+1] - allDistToEachNode[i])/ allSpeedsOnEachNode[i+1]);
        }

        return allTimesToEachNode;
    }
    
    void printShortestPath(int start, int end, vector<tuple<int,int>> prev, Graph& graph)
    {
        cout << "Shortest path:" << endl;
        if (!allPassedNodes.empty()) {
            for (size_t i = 0; i < allPassedNodes.size(); ++i) {
                cout << allPassedNodes[i]->getId();
                if (i != allPassedNodes.size() - 1) {
                    cout << " -> ";
                }
            }
            cout << endl;
        } else {
            cout << "No path exists from " << start << " to " << end << endl;
        }
    }

    vector<double> getSpeedsOnEachNode(Graph &graph)
    {
        vector<double> allSpeedsOnEachNode;
        double currentSpeed = speed;

        for(int i=0; i<allPassedNodes.size(); i++)
        {
            if(allPassedNodes[i]->getClassName() == "Path")
            {
                currentSpeed = min(speed, dynamic_cast<Path *>(allPassedNodes[i].get())->calculateAverageSpeed());
                allSpeedsOnEachNode.push_back(currentSpeed);
            }
            else if(allPassedNodes[i]->getClassName() == "Intersection")
            {
                Intersection *intersection = dynamic_cast<Intersection *>(allPassedNodes[i].get());
                currentSpeed = min(speed, intersection->calcualteAvgSpeedFromNodes(allPassedNodes[i-1], allPassedNodes[i+1]));
                allSpeedsOnEachNode.push_back(currentSpeed);
            }
            else if(allPassedNodes[i]->getClassName() == "Location")
            {
                allSpeedsOnEachNode.push_back( i!=0 ? allSpeedsOnEachNode.back() : 0);
            }
        }
        return allSpeedsOnEachNode;
    }

    int getIndexOfNodeId(Graph& graph, string id)
    {
        for(int i=0; i<graph.getNodes().size(); i++)
        {
            if(graph.getNodes()[i]->getId() == id)
            {
                return i;
            }
        }
        return -1;
    }

    int getIndexOfNode(Graph& graph, shared_ptr<Node> node)
    {
        for(int i=0; i<graph.getNodes().size(); i++)
        {
            if(graph.getNodes()[i] == node)
            {
                return i;
            }
        }
        return -1;
    }
    //-------------------------------------------------------------------------------------------------

public:

    vector<shared_ptr<Node>> dijkstra(Graph& graph, string type, string start)
    {
        int n = graph.getNodes().size();
        int src = getIndexOfNodeId(graph, start);
        int end = getIndexOfNodeId(graph, stop);
        double currentSpeed = speed;

        allTimesToEachNode.clear();
        allPassedNodes.clear();
        allSpeedsOnEachNode.clear();

        vector<double> weight(n, INT_MAX);
        vector<bool> sptSet(n, false);
        vector<tuple<int,int>> prev(n, {-1,-1});

        weight[src] = 0;

        shared_ptr<Node> prevNode;
        int prevNodeIndex;

        for (int count = 0; count < n - 1; count++) {
            int u = minValueIndex(weight, sptSet);

            sptSet[u] = true;

            for (int v = 0; v < n; v++)
            {
                // ukoliko nailazimo opet na cvor raskrsnice koji je procesuiran ponovo ga procesiramo jer mozda postoji jos prelaza kojim nismo mogli ici prvi put
                if(graph.getNodes()[u]->getClassName() == "Path" && graph.getNodes()[v]->getClassName() == "Intersection"  && graph.getMatrix()[u][v] && sptSet[v] == true)
                {
                    prevNodeIndex = u;
                    currentSpeed = min(speed, dynamic_cast<Path *>(graph.getNodes()[u].get())->calculateAverageSpeed());
                    prevNode = graph.getNodes()[u];
                    vector<double> newRow(n, 0);
                    Intersection *intersection = dynamic_cast<Intersection *>(graph.getNodes()[v].get());

                    for(int i=0; i<intersection->getListOfCrossSections().size(); i++)
                    {
                        if(get<0>(intersection->getListOfCrossSections()[i]) == prevNode)
                        {
                            int index=getIndexOfNode(graph, get<1>(intersection->getListOfCrossSections()[i]));
                            if(type == "fastest")
                            {
                                newRow[index] = weight[u] - weight[v] + get<2>(intersection->getListOfCrossSections()[i]) / min(speed, intersection->calculateAverageSpeedOnCrossSection(i)) + dynamic_cast<Path *>(graph.getNodes()[u].get())->getPathLenght() / currentSpeed; 
                            }
                            else if(type == "shortest")
                            {
                                newRow[index] = weight[u] - weight[v] + get<2>(intersection->getListOfCrossSections()[i]) + dynamic_cast<Path *>(graph.getNodes()[u].get())->getPathLenght();
                            }
                            
                        }
                    }
                    sptSet[v] = false;
                    count--;
                    graph.setRowOfMatrix(v,newRow);
                }
                if (!sptSet[v] && graph.getMatrix()[u][v] && weight[u] != INT_MAX)
                {
                    if(graph.getNodes()[u]->getClassName() == "Location")
                    {
                        weight[v] = weight[u];
                        prev[v] = {u,-1};
                    }
                    else if(graph.getNodes()[u]->getClassName() == "Path")
                    {
                        currentSpeed = min(speed, dynamic_cast<Path *>(graph.getNodes()[u].get())->calculateAverageSpeed());
                        double change;
                        if(type == "fastest")
                        {
                            change = dynamic_cast<Path *>(graph.getNodes()[u].get())->getPathLenght() / currentSpeed;
                        }
                        else if(type == "shortest")
                        {
                            change = dynamic_cast<Path *>(graph.getNodes()[u].get())->getPathLenght();
                        }
                        if(weight[u] + change < weight[v])
                        {
                            prevNode = graph.getNodes()[u];
                            weight[v] = weight[u] + change;
                            prev[v] = {u,-1};

                            if(graph.getNodes()[v]->getClassName() == "Intersection")
                            {
                                prevNodeIndex = u;
                                vector<double> newRow(n, 0);
                                Intersection *intersection = dynamic_cast<Intersection *>(graph.getNodes()[v].get());

                                for(int i=0; i<intersection->getListOfCrossSections().size(); i++)
                                {
                                    if(get<0>(intersection->getListOfCrossSections()[i]) == prevNode)
                                    {
                                        int index=getIndexOfNode(graph, get<1>(intersection->getListOfCrossSections()[i]));
                                        if (type == "fastest")
                                        {
                                            newRow[index] = get<2>(intersection->getListOfCrossSections()[i]) / min(speed, intersection->calculateAverageSpeedOnCrossSection(i));
                                        }
                                        else if (type == "shortest")
                                        {
                                            newRow[index] = get<2>(intersection->getListOfCrossSections()[i]);
                                        }
                                    }
                                }
                                graph.setRowOfMatrix(v,newRow);
                            }
                        }
                    }
                    else if(graph.getNodes()[u]->getClassName() == "Intersection" && weight[u] + graph.getMatrix()[u][v] < weight[v])
                    {
                        weight[v] = weight[u] + graph.getMatrix()[u][v];
                        prev[v] = {u,prevNodeIndex};
                    }
                }
            }
        }
        this->allPassedNodes = getShortestPathNodes(src, end, prev, graph);

        this->allSpeedsOnEachNode = getSpeedsOnEachNode(graph);

        if(type == "fastest")
        {
            this->allTimesToEachNode = getTimesToEachNode(src, end, prev, weight, graph);
            totalDistance = getTotalDistanceBasedOnSpeedsOnEachNode();
        }
        else if(type == "shortest")
        {
            this->allTimesToEachNode = getTimesToEachNodeFromDist(src, end, prev, weight, graph);
            totalDistance = weight[end];
        }

        return allPassedNodes;
    }
};
