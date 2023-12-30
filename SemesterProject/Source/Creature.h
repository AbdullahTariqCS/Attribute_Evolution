#pragma once
#include "Backend.h"

 
class CreatureFtn;
struct Attrib
{
	float max;
	float min;
	float current;
	float mutationIndex;
	
	Attrib() {}
	Attrib(float _max, float _min, float curr, float mtIdx) :
		max(_max), min(_min), current(curr), mutationIndex(mtIdx) {}

	Attrib(float _max, float _min, float mtIdx) :
		max(_max), min(_min), current(hf::avg(max, min)), mutationIndex(mtIdx) {}

	inline void operator = (const Attrib& n)
	{
		max = n.max; 
		min = n.min; 
		current = n.current; 
		mutationIndex = n.mutationIndex; 
	}

	
};


class Creature
{
	Attrib m_Speed;
	Attrib m_Energy;
	Attrib m_Size;
	Attrib m_DeltaEnergy; 

	Dimension m_Dimension;
	Coords m_Coords;
	Direction m_Direction;
	DirectionProb m_DirectionProb; //varies on the basis of where it gets the most energy and prevents death

	Creature* m_InChase; //tells if the creature is in chase with another one. null if not

	bool dead; 
	float m_DeltaTime;
	float m_MutationIndex; //from 0 to 1. percentage of variation in attributes when passing on the next generation
	float m_EnergyThreshold; //front 0 to 1. at what energy percentage the creature should start worrying about conserving it

public: 
	Creature() {}
	Creature(Dimension dimension, float deltaTime);
	void setAttributes(Attrib speed, Attrib energy, Attrib deltaEnergy, float energyThreshold, DirectionProb directionProb);


private:
	friend class World;
	friend class CreatureFtn;

	//setters
	
	void setAttributes(const Creature& newCreature); 
	void setPosition(Coords coords, Direction direction); 
	void setRandomPosition(Dimension dim); 
	void operator = (const Creature& n); 
	static void updateCoords(Creature& cr);
	static void isChildOf(Creature& cr, const Creature& p1, const Creature& p2);
	
	static Attrib inherit(const Attrib& p1, const Attrib& p2, bool constantRange);
	static void changeDirProbability(Creature& cr, bool increment, Direction dir);
	void changeEnergy(bool increment, float current); 
	bool energyDepleted();
	bool canMate(); 
	
	

};
