# Traffic Simulation System

A comprehensive C++ traffic simulation system that models vehicle movement through a network of roads, intersections, and locations. The system implements pathfinding algorithms and supports both static and dynamic routing strategies.

## Features

- **Graph-based road network modeling** with nodes representing locations, paths, and intersections
- **Multiple pathfinding algorithms** - shortest path and fastest path using Dijkstra's algorithm
- **Dynamic vehicle routing** - vehicles can recalculate routes at intersections based on current traffic conditions
- **Traffic congestion simulation** - paths and intersections have capacity limits affecting vehicle speeds
- **Real-time simulation** - step-by-step execution with configurable time steps
- **Interactive simulation control** - run, pause, and monitor simulation progress

## Architecture

The system follows object-oriented design principles with the following key components:

### Core Classes

- **Node** (abstract base class) - Represents any point in the road network
  - **Location** - Start/end points for vehicles
  - **Path** - Road segments with length, speed limits, and capacity
  - **Intersection** - Junction points with multiple cross-sections and traffic management

- **Vehicle** - Base class for all vehicles with pathfinding capabilities
  - **DynamicVehicle** - Advanced vehicles that can recalculate routes during simulation

- **Graph** - Represents the entire road network with adjacency matrix
- **Simulation** - Manages the simulation execution and vehicle movement

### Key Algorithms

- **Dijkstra's Algorithm** - Implemented for both shortest path and fastest path calculations
- **Dynamic Routing** - Real-time path recalculation based on traffic conditions
- **Traffic Flow Modeling** - Speed reduction based on congestion levels

## Getting Started

### Prerequisites

- C++ compiler with C++11 support or later
- Standard Template Library (STL)

### Compilation

```bash
g++ -std=c++11 -o traffic_simulation main.cpp
```

### Input Format

The system reads road network configuration from a text file. Example format:

```
A B 1 2 3 4 5 6 #1 #2
Location A: 1
Location B: 4
Path 1: 500 100 60 2 #1
Path 2: 500 100 60 2 A
Path 3: 500 100 60 2 #2
Path 4: 500 100 60 2 #1
Path 5: 400 100 90 2 #2
Path 6: 500 100 60 2 B
Intersection #1: 2 5
    1->2 100 100
    1->3 100 100
    1->5 100 100
    4->2 100 100
    4->3 100 100
Intersection #2: 2 2
    3->6 100 100
    5->6 100 100
```

### Running the Simulation

```bash
./traffic_simulation
```

The program will load the network from `input.txt` and start an interactive simulation with the following options:

1. **Run simulation step** - Advance the simulation by one time step
2. **Print current state of vehicles** - Display vehicle positions and progress
3. **Print average time** - Show average travel time between specified points
4. **Check if simulation is over** - Verify if all vehicles have reached their destinations

## Usage Examples

### Creating Vehicles

```cpp
// Static routing vehicles
Vehicle v1("A", "B", 100, "shortest");  // Shortest path, 100 m/s max speed
Vehicle v2("B", "A", 80, "fastest");    // Fastest path, 80 m/s max speed

// Dynamic routing vehicle
DynamicVehicle v3("A", "B", 90, "fastest");  // Recalculates route at intersections
```

### Setting up Simulation

```cpp
Graph graph;
// Load graph from file...

vector<Vehicle*> vehicles = {&v1, &v2, &v3};
Simulation simulation(&graph, vehicles, 1.0);  // 1 second time step
```

## Network Components

### Paths
- **Length**: Physical distance in meters
- **Max Speed**: Speed limit for the path
- **Capacity**: Maximum number of vehicles
- **Scalar**: Congestion factor affecting speed reduction

### Intersections
- **Cross-sections**: Define valid vehicle transitions
- **Capacity**: Maximum vehicles that can wait at intersection
- **Dynamic speed calculation**: Based on current traffic load

### Traffic Modeling

The system implements realistic traffic behavior:
- **Speed reduction** due to congestion on paths
- **Intersection delays** based on traffic density
- **Dynamic routing** for adaptive vehicles
- **Realistic vehicle movement** with proper acceleration/deceleration

## File Structure

```
├── main.cpp              # Main simulation program
├── Graph.h               # Road network representation
├── Node.h                # Abstract base class for network nodes
├── Location.h            # Start/end points
├── Path.h                # Road segments
├── Intersection.h        # Junction points
├── Vehicle.h             # Base vehicle class with pathfinding
├── DynamicVehicle.h      # Advanced vehicle with dynamic routing
├── Simulation.h          # Simulation management
└── input.txt             # Network configuration file
```

## Algorithm Details

### Dijkstra Implementation
- Supports both distance-based (shortest) and time-based (fastest) pathfinding
- Handles complex intersection logic with multiple cross-sections
- Dynamic weight adjustment based on current traffic conditions

### Dynamic Routing
- Triggered when vehicles reach intersections
- Recalculates optimal path from current position
- Maintains continuity of traveled distance and time
