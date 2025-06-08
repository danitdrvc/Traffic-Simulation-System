#pragma once
#include <string>

using namespace std;

class Node
{
private:
    const string id;

public:
    Node(const string id) : id(id) {}

    const string getId() const
    {
        return this->id;
    }

    virtual string getClassName() const = 0;
};