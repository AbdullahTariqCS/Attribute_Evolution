#include <SFML/Graphics.hpp>
#include "World.h"
int main() {
    
    WorldAttribs attrib;
    attrib.deltaTime = (float)1000 / (float)30; 
    attrib.dim.width = 800.0f; 
    attrib.dim.height = 800.0f; 
    attrib.grassDensity = 0.1f; 
    attrib.grassSpawnRate = 4; 
    attrib.initialPopulation = 30; 
    attrib.gridCellSize = 10.0f;

    Creature Predetor;
    Creature Prey; 

    Prey.setAttributes(Attrib(4.0f, 0.5f, 0.05), Attrib(10.0f, 0.0f, 0.05f), Attrib(0.01f, 0.01f, 0.05f), 0.3f, DirectionProb());

    Predetor.setAttributes(Attrib(4.0f, 0.5f, 0.05), Attrib(10.0f, 0.0f, 0.05f), Attrib(0.01f, 0.01f, 0.05f), 0.3f, DirectionProb());
    
    World world(attrib); 
    world.initializePopulation(Predetor, Prey); 

    world.run(); 


    return 0;
}