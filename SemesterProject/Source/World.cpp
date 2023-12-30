#include "World.h"

World::World(WorldAttribs attribs) :m_Attribs(attribs)
{

	srand(time(0)); 
}


void World::initializePopulation(Creature& Predetor, Creature& Prey)
{
	for (int i = 0; i < m_Attribs.initialPopulation; i++)
	{
		pushBack(Predetor, Prey); 
	}
	/*for (float x = 5.0f; x <= m_Attribs.dim.width; x += 10.0f)
	{
		for (float y = 5.0f; y <= m_Attribs.dim.width; y += 10.0f)
		{
			m_Grass.add(Coords(x, y), (int)(pow(m_Attribs.gridCellSize, 2) * m_Attribs.grassDensity));
		}
	}*/
}

void World::run()
{
	sf::RenderWindow window(sf::VideoMode(m_Attribs.dim.width, m_Attribs.dim.height), "Area");

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		iterate(); 
		/*for (float x = 5.0f; x <= m_Attribs.dim.width; x += 10.0f)
		{
			for (float y = 5.0f; y <= m_Attribs.dim.width; y += 10.0f)
			{
				std::vector<sf::CircleShape> shape = m_Grass.find(Coords(x, y));
				for (sf::CircleShape sh : shape)
				{
					window.draw(sh); 
				}
			}
		}*/
		for (auto itr = m_Prey.begin(); itr != m_Prey.end(); itr++)
		{
			Creature prey = *itr; 

			sf::CircleShape shape; 
			shape.setRadius(4.0f); 
			shape.setFillColor(sf::Color::White); 
			shape.setPosition(prey.m_Coords.x, prey.m_Coords.y); 
			window.draw(shape); 
		}
		for (auto itr = m_Predetor.begin(); itr != m_Predetor.end(); itr++)
		{
			Creature predetor = *itr; 

			sf::CircleShape shape;
			shape.setRadius(6.0f);
			shape.setFillColor(sf::Color::Red);
			shape.setPosition(predetor.m_Coords.x, predetor.m_Coords.y); 
			window.draw(shape); 
		}
		
		window.display();
		Sleep(m_Attribs.deltaTime); 
	}
}

void World::iterate()
{
	

	std::list<Creature>::iterator currentPredetor = m_Predetor.begin(); 
	std::list<Creature>::iterator currentPrey = m_Prey.begin(); 
	std::list<grass::info>::iterator currentCell = m_DepletedGrass.begin();
	std::list <std::pair<Creature*, Creature*>>::iterator currentChase = m_inChase.begin();

	/*while (currentPredetor != m_Predetor.end() && currentPrey != m_Prey.end() && currentCell != m_DepletedGrass.end() && currentChase != m_inChase.end())
	{
	
		iteratePredetor(currentPredetor, inChase); 
		iteratePrey(currentPrey); 
		iterateGrass(currentCell); 
		iterateChase(currentChase); 

		currentPredetor++;
		currentPrey++;
		currentCell++;
		currentChase++;
	}*/
	while (currentPredetor != m_Predetor.end())
	{
		bool condition = iteratePredetor(currentPredetor);
		if (!condition)
		{
			auto temp = std::next(currentPredetor);
			m_Predetor.erase(currentPredetor);
			currentPredetor = temp;

		}
		else
			currentPredetor++;
			
	}
	while (currentPrey != m_Prey.end())
	{
		bool condition = iteratePrey(currentPrey);
		if (!condition)
		{
			auto temp = std::next(currentPrey);
			m_Prey.erase(currentPrey);
			currentPrey = temp;
		}
		else
			currentPrey++;
	}
	/*while (currentCell != m_DepletedGrass.end())
	{
		iterateGrass(currentCell);
		++currentCell;
	}*/
	/*while (currentChase != m_inChase.end())
	{
		iterateChase(currentChase);
		currentChase++;
	}*/
	
		 

}

//void World::loadBuffers(std::vector<Coords> Grass, std::vector<Coords> Predetors, std::vector<Coords> Prey)
//{
//	Grass.clear(); 
//	Predetors.clear(); 
//	Prey.clear(); 
//
//	auto preyItr = m_Prey.begin(); 
//	auto predetorItr = m_Predetor.begin(); 
//
//	for (float x = 5.0f; x <= m_Attribs.dim.width; x += 10.0f)
//	{
//		if (predetorItr != m_Predetor.end())
//		{
//			Predetors.push_back((*predetorItr).m_Coords); 
//			predetorItr++;
//		}
//		if (preyItr != m_Prey.end())
//		{
//			Prey.push_back((*preyItr).m_Coords); 
//			preyItr++;
//		}
//		for (float y = 5.0f; y <= m_Attribs.dim.width; y += 10.0f)
//		{
//			m_Grass.loadBuffer(Coords(x, y), Grass); 
//		}
//	}
//	for (auto itr = m_Predetor.begin(); itr != m_Predetor.end(); itr++)
//	{
//		Predetors.push_back((*itr).m_Coords);
//	}
//	for (auto itr = m_Prey.begin(); itr != m_Prey.end(); itr++)
//	{
//		Predetors.push_back((*itr).m_Coords);
//	}
//}

bool World::iteratePrey(std::list<Creature>::iterator Prey)
{
	Creature& prey = *Prey;
	if (prey.dead)
		return false;

	if (prey.m_InChase)//don't process if the prey is in chase
		return true;

	if (prey.m_Energy.current == prey.m_Energy.min)
	{
		if (prey.m_InChase)
			prey.m_InChase->m_InChase == nullptr;
		return false;
	}
	
	updatePosition(*Prey, Type::Prey); 
	
	Creature* Predetor = m_PredetorArea.find(prey.m_Coords); 
	Creature* mate = m_PreyArea.find(prey.m_Coords);

	if (m_Grass.find(prey.m_Coords).size() != 0)
	{
		prey.changeEnergy(true, prey.m_DeltaEnergy.current); 
		prey.changeDirProbability(prey, true, prey.m_Direction); 

		m_Grass.remove(prey.m_Coords); 
		m_DepletedGrass.push_back(grass::info(prey.m_Coords, m_Attribs.deltaTime * m_Attribs.grassSpawnRate));
	}

	//initialize chase 
	/*if (Predetor)
	{
		prey.m_InChase = Predetor; 
		Predetor->m_InChase = &prey;
		prey.changeDirProbability(prey, false, prey.m_Direction); 

		m_inChase.push_back(std::make_pair(Predetor, &prey));

		updatePosition(prey, Type::Prey); 
		updatePosition(*Predetor, Type::Predetor);
	}*/
	if(mate && prey.canMate() && mate->canMate())
	{
		Creature newPray(m_Attribs.dim, m_Attribs.deltaTime); 
		newPray.isChildOf(newPray, prey, *mate); 
		prey.changeEnergy(false, prey.m_DeltaEnergy.max); 
		mate->changeEnergy(false, mate->m_DeltaEnergy.max); 
		pushBack(newPray, Type::Prey); 

	}

	return true;

}

bool World::iteratePredetor(std::list<Creature>::iterator Predetor)
{
	Creature& predetor = *Predetor;
	if (predetor.dead)
		return false;

	if (predetor.m_InChase)
		return true;

	if (predetor.m_Energy.current == predetor.m_Energy.min)
	{
		if (predetor.m_InChase)
		{
			predetor.m_InChase->m_InChase = nullptr;
			
		}
		return false;
	}

	updatePosition(predetor, Type::Predetor);
	Creature* prey = m_PreyArea.find(predetor.m_Coords);
	Creature* mate = m_PreyArea.find(predetor.m_Coords);

	


	//initiate chase
	/*if (prey)
	{
		predetor.m_InChase = prey;
		prey->m_InChase = &predetor;

		predetor.changeDirProbability(predetor, true, predetor.m_Direction);

		m_inChase.push_back(std::make_pair(&predetor, prey));
		updatePosition(predetor, Type::Predetor); 
		updatePosition(*prey, Type::Prey); 

	}*/
	if (mate && predetor.canMate() && mate->canMate())
	{
		Creature newPredetor(m_Attribs.dim, m_Attribs.deltaTime);
		Creature().isChildOf(newPredetor, predetor, *mate);

		predetor.changeEnergy(false, predetor.m_DeltaEnergy.max);
		mate->changeEnergy(false, mate->m_DeltaEnergy.max);

		pushBack(newPredetor, Type::Predetor);

	}
	return true;

	

}

void World::iterateChase(std::list<std::pair<Creature*, Creature*>>::iterator inChase)
{
	std::pair<Creature*, Creature*> pair = *inChase;
	Creature* Predetor = pair.first;
	Creature* Prey = pair.second; 

	if (Predetor->dead || Prey->dead)
		return;

	updatePosition(*Predetor, Type::Predetor); 
	updatePosition(*Prey, Type::Prey); 
	
	if (Predetor->m_Energy.current < Predetor->m_Energy.min)
	{
		Predetor->dead = true;
		Predetor->m_InChase = nullptr;
		Prey->m_InChase = nullptr; 
	}
	else if (m_PredetorArea.hash(Predetor->m_Coords) == m_PredetorArea.hash(Prey->m_Coords))
	{
		Predetor->changeDirProbability(*Predetor, true, Predetor->m_Direction); 
		Predetor->changeEnergy(true, Predetor->m_DeltaEnergy.max);
		Predetor->m_InChase = nullptr;
		Prey->m_InChase = nullptr;
		Prey->dead = true;

	}
}

void World::iterateGrass(std::list<grass::info>::iterator cell)
{
	grass::info& currentCell = *cell;
	if (currentCell.remSpawnTime == 0)
	{
		m_Grass.add(currentCell.coords, (int)pow(m_Attribs.gridCellSize, 2) * m_Attribs.grassDensity);
		m_DepletedGrass.erase(cell); 
	}
	currentCell.remSpawnTime -= m_Attribs.deltaTime;	
}

void World::updatePosition(Creature& creature, Type type)
{
	Coords prevCoords = creature.m_Coords; 
	Creature().updateCoords(creature); 
	if (creature.m_Coords.x / 10 == prevCoords.x / 10 && creature.m_Coords.y / 10 == prevCoords.y / 10 && creature.m_InChase)
		return;
	switch (type)
	{
	case Type::Predetor: 
	{
		m_PredetorArea.remove(prevCoords, &creature); 
		m_PredetorArea.add(creature.m_Coords, &creature); 
		break;
	}
	case Type::Prey: 
	{
		m_PredetorArea.remove(prevCoords, &creature); 
		m_PreyArea.add(creature.m_Coords, &creature); 
		break;
	}
	}
	

}

void World::pushBack(Creature& Predetor, Creature& Prey)
{
	pushBack(Predetor, Type::Predetor); 
	pushBack(Prey, Type::Prey); 

}

void World::pushBack(Creature& creature, Type type)
{
	switch (type)
	{
	case Type::Predetor: 
	{
		Creature Predetor(m_Attribs.dim, m_Attribs.deltaTime);
		Predetor.setAttributes(creature);
		Predetor.setRandomPosition(m_Attribs.dim); 
		m_Predetor.push_back(Predetor);
		m_PredetorArea.add(Predetor.m_Coords, &m_Predetor.back());
		break;
	}
	case Type::Prey:
	{
		Creature Prey(m_Attribs.dim, m_Attribs.deltaTime);
		Prey.setAttributes(creature);
		Prey.setRandomPosition(m_Attribs.dim);

		m_Prey.push_back(Prey);
		m_PreyArea.add(Prey.m_Coords, &m_Predetor.back());
		break;
	}
	}
}

