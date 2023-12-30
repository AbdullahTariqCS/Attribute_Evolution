#pragma once
#include "Map.h"

enum class Type {
	None, Predetor, Prey
};

struct WorldAttribs
{
	Dimension dim; 
	float grassDensity; //number of grass per grid
	float grassSpawnRate;
	float gridCellSize;

	int initialPopulation;
	float conflictBox;
	float deltaTime;

};


class World
{
	
	WorldAttribs m_Attribs; 

	creat::Map m_PredetorArea; 
	creat::Map m_PreyArea; 
	grass::Map m_Grass; 

	std::list<grass::info> m_DepletedGrass;

	std::list<Creature> m_Predetor; 
	std::list<Creature> m_Prey; 
	std::list <std::pair<Creature*, Creature*>> m_inChase;


public: 

	World(WorldAttribs attribs); 
	void initializePopulation(Creature& Predetor, Creature& Prey);//pass in the attributes of initial creatures using
	void run(); 

	void iterate(); 

private: 
	bool iteratePrey(std::list<Creature>::iterator Prey);
	bool iteratePredetor(std::list<Creature>:: iterator Predetor);

	void iterateChase(std::list<std::pair<Creature*, Creature*>>::iterator inChase); 
	void iterateGrass(std::list<grass::info>::iterator cell);

	void updatePosition(Creature& creature, Type type); 
	void pushBack(Creature& Predetor, Creature& Prey); 
	void pushBack(Creature& creature, Type type); 
};