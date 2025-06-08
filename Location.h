#pragma once
#include "Node.h"
#include <vector>

using namespace std;

class Location : public Node
{
public:
    Location(string id) : Node(id) {}

    string getClassName() const
    {
        return "Location";
    }
};