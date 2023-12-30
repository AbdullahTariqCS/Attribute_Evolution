#include "Map.h"

std::string creat::Map::hash(Coords coords)
{
	int x1 = (coords.x / 10) * 10;
	int y1 = (coords.y / 10) * 10;
	int x2 = (coords.x / 10 + 1) * 10;
	int y2 = (coords.y / 10 + 1) * 10;

	/*std::pair<float, float> pair1 = std::make_pair((float)x1, (float)y1);
	std::pair<float, float> pair2 = std::make_pair((float)x2, (float)y2);
	return std::make_pair(pair1, pair2);*/

	return std::to_string((float)x1) + std::to_string((float)y1) + std::to_string((float)x2) + std::to_string((float)y2); 
}
std::string grass::Map::hash(Coords coords)
{
	int x1 = (coords.x / 10) * 10;
	int y1 = (coords.y / 10) * 10;
	int x2 = (coords.x / 10 + 1) * 10;
	int y2 = (coords.y / 10 + 1) * 10;

	/*std::pair<float, float> pair1 = std::make_pair((float)x1, (float)y1);
	std::pair<float, float> pair2 = std::make_pair((float)x2, (float)y2);
	return std::make_pair(pair1, pair2);*/

	return std::to_string((float)x1) + std::to_string((float)y1) + std::to_string((float)x2) + std::to_string((float)y2); 
}

Creature* creat::Map::find(Coords coords)
{
	
	if (m_Map.find(hash(coords)) != m_Map.end())
		return *m_Map[hash(coords)].begin();
	return nullptr;
}

void creat::Map::remove(Coords coords, Creature* creature)
{
	if (m_Map.find(hash(coords)) == m_Map.end())
		return;

	m_Map[hash(coords)].erase(creature);

	if (m_Map[hash(coords)].empty())
		m_Map.erase(hash(coords));

}

void creat::Map::add(Coords coords, Creature* creature)
{
	m_Map[hash(coords)].insert(creature);
}

std::vector<sf::CircleShape>& grass::Map::find(Coords coords)
{
	if (m_Map.find(hash(coords)) == m_Map.end())
	{
		std::vector<sf::CircleShape> null;
		return null;
	}

	return m_Map[hash(coords)]; 
}

void grass::Map::remove(Coords coords)
{
	m_Map.erase(hash(coords)); 
}

void grass::Map::add(Coords coords, int num, float spanwTimeRemaining)
{
	m_RemainingSpawnTime = spanwTimeRemaining;
	m_Map[hash(coords)] = std::vector<sf::CircleShape>(); 

	int x1 = (coords.x / 10) * 10;
	int y1 = (coords.y / 10) * 10;
	int x2 = (coords.x / 10 + 1) * 10;
	int y2 = (coords.y / 10 + 1) * 10;

	for (int i = 0; i < num; i++)
	{
		sf::CircleShape g; 
		g.setRadius(0.1f); 
		g.setFillColor(sf::Color::Green); 
		g.setPosition(hf::randf(x1, x2), hf::randf(y1, y2));

		m_Map[hash(coords)].push_back(g);
	}

}

void grass::Map::add(Coords coords, int num)
{
	add(coords, num, 0); 
}

//void grass::Map::loadBuffer(Coords coords, std::vector<Coords>& buffer)
//{
//	if(m_Map.find(hash(coords)) == m_Map.end())
//		return;
//
//	int x1 = (coords.x / 10) * 10;
//	int y1 = (coords.y / 10) * 10;
//	int x2 = (coords.x / 10 + 1) * 10;
//	int y2 = (coords.y / 10 + 1) * 10;
//
//	for (int i = 0; i < m_Map[hash(coords)]; i++)
//	{
//		buffer.push_back({ hf::randf(x1, x2), hf::randf(y1, y2) });
//	}
//}

std::unordered_map<std::string, std::vector<sf::CircleShape>>::iterator grass::Map::iterator()
{
	return m_Map.begin();
}


