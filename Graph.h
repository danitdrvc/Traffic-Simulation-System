#pragma once
#include "Node.h"
#include "Intersection.h"
#include "Path.h"
#include "Location.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <memory>

using namespace std;

class Graph
{
private:
    vector<shared_ptr<Node>> nodes;
    vector<vector<double>> matrix;

public:
    Graph(vector<shared_ptr<Node>> nodes, vector<vector<double>> matrix) : nodes(nodes), matrix(matrix) {}
    Graph() {}

    vector<shared_ptr<Node>> getNodes() const
    {
        return nodes;
    }

    vector<vector<double>> getMatrix() const
    {
        return matrix;
    }

    void setMatrix(vector<vector<double>> matrix)
    {
        this->matrix = matrix;
    }

    void setRowOfMatrix(int row, vector<double> newRow)
    {
        matrix[row] = newRow;
    }

    friend istream& operator>>(istream& is, Graph& graph)
    {
        string line;
        getline(is, line);
        istringstream iss1(line);
        vector<string> nodeIds;

        while(!iss1.eof())
        {
            string nodeId;
            iss1 >> nodeId;
            nodeIds.push_back(nodeId);
        }
        
        int i=0;
        while (getline(is, line) && !line.empty())
        {
            string firstWord;
            istringstream iss(line);
            iss >> firstWord;
            string goingToId = "";
            vector<double> row(nodeIds.size(), 0);

            if (firstWord == "Location")
            {
                string id;
                iss >> id >> goingToId;
                id.erase(id.size() - 1);
                graph.nodes.push_back(make_shared<Location>(id));
            }

            else if (firstWord == "Path")
            {
                string id;
                double pathLenght;
                double maxSpeed;
                int maxNumVehicles;
                int scalar;
                iss >> id >> pathLenght >> maxSpeed >> scalar >> maxNumVehicles >> goingToId;
                id.erase(id.size() - 1);
                graph.nodes.push_back(make_shared<Path>(id, pathLenght, maxSpeed, maxNumVehicles, scalar));
            }

            else if (firstWord == "Intersection")
            {
                string id;
                int numVehicles;
                int numOfCrossSections;
                iss >> id >> numVehicles >> numOfCrossSections;
                id.erase(id.size() - 1);
                vector<tuple<shared_ptr<Node>, shared_ptr<Node>, double, double>> listOfCrossSections;
                for(int i=0; i<numOfCrossSections; i++)
                {
                    getline(is, line);
                    istringstream iss(line);
                    string path;
                    iss >> path;
                    string inwardPathId = path.substr(0, path.find("->"));
                    shared_ptr<Node> inwardPathNode = nullptr;
                    string outwardPathId = path.substr(path.find("->") + 2, path.length());
                    shared_ptr<Node> outwardPathNode = nullptr;

                    for(int i=0; i<nodeIds.size(); i++)
                    {
                        if(nodeIds[i] == inwardPathId)
                        {
                            inwardPathNode = graph.nodes[i];
                        }
                        if(nodeIds[i] == outwardPathId)
                        {
                            outwardPathNode = graph.nodes[i];
                            row[i] = 1;
                        }
                    }
                    double distance;
                    double speed;
                    iss >> distance >> speed;

                    listOfCrossSections.push_back(make_tuple(inwardPathNode, outwardPathNode, distance, speed));
                }
                graph.nodes.push_back(make_shared<Intersection>(id, listOfCrossSections, numVehicles));
            }

            
            if (goingToId != "")
            {
                for(int j=0; j<nodeIds.size(); j++)
                {
                    if(nodeIds[j] == goingToId)
                    {
                        row[j] = 1;
                    }
                }
            }
            graph.matrix.push_back(row);
            i++;
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Graph graph)
	{
        for(int i=0; i<graph.getNodes().size(); i++)
        {
            os << graph.getNodes()[i]->getId() << " ";
        }
        os << endl;

        for(int i=0; i<graph.getNodes().size(); i++)
        {
            os << graph.getNodes()[i]->getClassName() << " " << graph.getNodes()[i]->getId() << ": ";

            if (graph.getNodes()[i]->getClassName() == "Location")
            {
                for(int j=0; j<graph.getMatrix().size(); j++)
                {
                    if(graph.getMatrix()[i][j] != 0)
                    {
                        os << graph.getNodes()[j]->getId() << endl;
                    }
                }
            }

            else if (graph.getNodes()[i]->getClassName() == "Path")
            {
                Path *path = dynamic_cast<Path *>(graph.getNodes()[i].get());
                os << path->getPathLenght() << " " << path->getMaxSpeed() << " " << path->getScalar() << " " << path->getMaxNumVehicles() << " ";
                for(int j=0; j<graph.getMatrix().size(); j++)
                {
                    if(graph.getMatrix()[i][j] != 0)
                    {
                        os << graph.getNodes()[j]->getId() << endl;
                    }
                }
            }

            else if (graph.getNodes()[i]->getClassName() == "Intersection")
            {
                Intersection *intersection = dynamic_cast<Intersection *>(graph.getNodes()[i].get());
                os << intersection->getMaxNumVehicles() << " " << intersection->getListOfCrossSections().size() << endl;
                for(int j=0; j<intersection->getListOfCrossSections().size(); j++)
                {
                    os << "\t" << get<0>(intersection->getListOfCrossSections()[j])->getId() << "->" << get<1>(intersection->getListOfCrossSections()[j])->getId() << " " << get<2>(intersection->getListOfCrossSections()[j]) << " " << get<3>(intersection->getListOfCrossSections()[j]) << endl;
                }
            }
        }
        return os;
    }
};