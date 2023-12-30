#pragma once
#include "Creature.h"



namespace creat {

	class Map
	{
		std::unordered_map<std::string, \
			std::unordered_set<Creature*>> m_Map;

	public:
		Creature* find(Coords coords);
		void remove(Coords coords, Creature* creature);
		void add(Coords coords, Creature* creature);
		std::string hash(Coords coords); 

	};
}

namespace grass {
	class Map
	{
		std::unordered_map < std::string, std::vector<sf::CircleShape> > m_Map;
		float m_RemainingSpawnTime;

	public:
		std::vector <sf::CircleShape> & find(Coords coords);
		void remove(Coords coords);
		void add(Coords coords, int num, float remainingTime);
		void add(Coords coords, int num);
		std::string hash(Coords coords); 
		//void loadBuffer(Coords coords, std::vector<Coords>& buffer);
		std::unordered_map< std::string, std::vector<sf::CircleShape>>::iterator iterator();

		/*void add(std::pair<std::pair<float, float>, std::pair<float, float>> cell, std::vector<Coords> grass);
		Map remove(std::pair<std::pair<float, float>, std::pair<float, float>> cell, std::vector<Coords> grass)*/
	};
	struct info
	{
		Coords coords;
		float remSpawnTime;
		info(Coords _coords, float time) : coords(_coords), remSpawnTime(time) {  }
	};
}
