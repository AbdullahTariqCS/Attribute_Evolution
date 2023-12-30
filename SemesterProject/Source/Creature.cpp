
#include "Creature.h"


Creature::Creature(Dimension dimension, float deltaTime)
{
	dead = false; 
	m_Dimension = dimension;
	m_DeltaTime = deltaTime;

}

void Creature::setAttributes(Attrib speed , Attrib energy, Attrib deltaEnergy, float energyThreshold, DirectionProb dirprob)
{
	m_Speed = speed;
	m_Energy = energy;
	m_DeltaEnergy = deltaEnergy;
	m_EnergyThreshold = energyThreshold;
	m_DirectionProb = dirprob;
}

void Creature::setAttributes(const Creature& newCreature)
{
	m_Speed = newCreature.m_Speed;
	m_Energy = newCreature.m_Energy;
	m_DeltaEnergy = newCreature.m_DeltaEnergy;
	m_EnergyThreshold = newCreature.m_EnergyThreshold;
	m_Size = newCreature.m_Size;
	m_DirectionProb = newCreature.m_DirectionProb;
}

void Creature::setPosition(Coords coords, Direction direction)
{
	m_Coords = coords;
	m_Direction = direction;
}

void Creature::setRandomPosition(Dimension dimensions)
{
	setPosition(
		Coords(hf::randf(0, dimensions.width), hf::randf(0, dimensions.height)),
		rollDice(DirectionProb())
	);
}

void Creature::updateCoords(Creature& cr)
{
	Coords newCoords;
	Coords incr[] = { Coords(0, 0), Coords(0, -1), Coords(0, 1), Coords(1, 0), Coords(-1, 0) };
	float speed; 

	if (cr.m_InChase)
	{
		cr.m_Direction = cr.m_InChase->m_Direction;
		speed = (cr.energyDepleted() ? cr.m_Speed.min : cr.m_Speed.max); 
		cr.changeEnergy(false, cr.m_DeltaEnergy.max);
	}

	else
	{
		cr.m_Direction = rollDice(cr.m_DirectionProb);
		cr.m_Speed.current = hf::interpolate(cr.m_Energy.current, cr.m_Energy.max, cr.m_Energy.min, cr.m_Speed.max, cr.m_Speed.min);

		speed = (cr.energyDepleted() ? cr.m_Speed.min : cr.m_Speed.current);
		cr.changeEnergy(false, cr.m_DeltaEnergy.current);
	}

	newCoords.x = std::fmod((cr.m_Coords.x + speed * cr.m_DeltaTime * incr[(int)cr.m_Direction].x) , cr.m_Dimension.width);
	newCoords.y = std::fmod((cr.m_Coords.y + speed * cr.m_DeltaTime * incr[(int)cr.m_Direction].y), cr.m_Dimension.height);

	

	cr.m_Coords = newCoords;

}

void Creature::changeEnergy(bool increment, float current)
{
	m_Energy.current += (increment ? 1 : -1) * (m_Energy.max - m_Energy.min) * (energyDepleted() ? m_DeltaEnergy.min : current);
}

void Creature::changeDirProbability(Creature& cr, bool increment, Direction dir)
{
	int sign = (increment ? 1 : -1);
	
	switch (dir)
	{
	case Direction::Up:
	{
		cr.m_DirectionProb.up += sign * 0.05f;
		cr.m_DirectionProb.down -= sign * 0.05f / 3.0f;
		cr.m_DirectionProb.left -= sign * 0.05f / 3.0f;
		cr.m_DirectionProb.right -= sign * 0.05f / 3.0f;
		break;
	}
	case Direction::Down:
	{
		cr.m_DirectionProb.up -= sign * 0.05f / 3.0f;
		cr.m_DirectionProb.down += sign * 0.05f;
		cr.m_DirectionProb.left -= sign * 0.05f / 3.0f;
		cr.m_DirectionProb.right -= sign * 0.05f / 3.0f;
		break;
	}
	case Direction::Left:
	{
		cr.m_DirectionProb.up -= sign * 0.05f / 3.0f;
		cr.m_DirectionProb.down -= sign * 0.05f / 3.0f;
		cr.m_DirectionProb.left += sign * 0.05f;
		cr.m_DirectionProb.right -= sign * 0.05f / 3.0f;
		break;
	}
	case Direction::Right:
	{
		cr.m_DirectionProb.up -= sign * 0.05f / 3.0f;
		cr.m_DirectionProb.down -= sign * 0.05f / 3.0f;
		cr.m_DirectionProb.left -= sign * 0.05f / 3.0f;
		cr.m_DirectionProb.right += sign * 0.05f;
		break;
	}

	}
}

bool Creature::energyDepleted()
{
	return m_Energy.current <= (m_Energy.max - m_Energy.min) * m_EnergyThreshold + m_Energy.min;
}

bool Creature::canMate()
{
	return m_Energy.current >= hf::avg(m_Energy.min, m_Energy.max); 
}

void Creature::isChildOf(Creature& cr, const Creature& p1, const Creature& p2)
{
	Attrib speed = inherit(p1.m_Speed, p2.m_Speed, true);
	//Attrib energy = inherit(p1.m_Energy, p2.m_Energy, false);;
	//Attrib size = inherit(p1.m_Size, p2.m_Size);
	Attrib size = p1.m_Size; 

	Attrib energy = p1.m_Energy; 
	Attrib deltaEnergy = inherit(p1.m_DeltaEnergy, p2.m_DeltaEnergy, true);


	Coords coords(hf::randf(0, cr.m_Dimension.width), hf::randf(0, cr.m_Dimension.height));
	DirectionProb dirProb(
		hf::avg(p1.m_DirectionProb.up, p2.m_DirectionProb.up),
		hf::avg(p1.m_DirectionProb.down, p2.m_DirectionProb.down),
		hf::avg(p1.m_DirectionProb.left, p2.m_DirectionProb.left),
		hf::avg(p1.m_DirectionProb.right, p2.m_DirectionProb.right)
	);

	Creature newCreature(cr.m_Dimension, cr.m_DeltaTime);
	cr.setAttributes(speed, energy, deltaEnergy, cr.m_EnergyThreshold, dirProb);
	//newCreature.setPosition(coords, rollDice(dirProb));

	//return newCreature;

}

Attrib Creature::inherit(const Attrib& p1, const Attrib& p2, bool constantRange)
{
	Attrib newAttrib;

	float mutation;
	mutation = hf::randf(-p1.mutationIndex, p1.mutationIndex);
	float p1Max = p1.max +(p1.max - p1.min)*mutation;

	mutation = (constantRange ? mutation : hf::randf(-p1.mutationIndex, p1.mutationIndex));
	float p1Min = p1.min + (p1.max - p1.min) * mutation;

	p1Min = (p1Min < 0 ? 0 : p1Min);
	p1Max = (p1Max < 0 ? 0 : p1Max);

	float p1Current = hf::avg(p1Min, p1Max);



	mutation = hf::randf(-p2.mutationIndex, p2.mutationIndex);
	float p2Max = p2.max + (p2.max - p2.min) * mutation;

	mutation = (constantRange ? mutation : hf::randf(-p1.mutationIndex, p1.mutationIndex));
	float p2Min = p2.min + (p2.max - p2.min) * mutation;

	p2Min = (p2Min < 0 ? 0 : p2Min);
	p2Max = (p2Max < 0 ? 0 : p2Max);

	float p2Current = hf::avg(p2Min, p2Max);

	newAttrib.max = hf::avg(p1Max, p2Max);
	newAttrib.min = hf::avg(p1Min, p2Min);
	newAttrib.current = hf::avg(p1Current, p2Current);
	newAttrib.mutationIndex = hf::avg(p1.mutationIndex, p2.mutationIndex);
	return newAttrib;
}

void Creature::operator=(const Creature& n)
{
	m_Speed = n.m_Speed;
	m_Energy = n.m_Energy;
	m_Size = n.m_Size;
	m_DeltaEnergy = n.m_DeltaEnergy;

	m_Dimension = n.m_Dimension;
	m_Coords = n.m_Coords;
	m_Direction = n.m_Direction;
	m_DirectionProb = n.m_DirectionProb;

	m_InChase = n.m_InChase;

	m_DeltaTime = n.m_DeltaTime;
	m_MutationIndex = n.m_MutationIndex;
	m_EnergyThreshold = n.m_EnergyThreshold;
}

