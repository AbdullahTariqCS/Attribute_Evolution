#include "Backend.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <random>
#include <time.h>
#include <cmath>

//std::pair<std::pair<float, float>, std::pair<float, float>> hash(float x, float y, float radius)
//{
//	int x1 = (x / 10) * 10; 
//	int y1 = (y / 10) * 10;
//	int x2 = (x / 10 + 1) * 10; 
//	int y2 = (y / 10 + 1) * 10; 
//
//
//	return std::make_pair(std::make_pair((float)x1, (float)x2), std::make_pair((float)x2, (float)y2));
//}

float hf::interpolate(float x, float max_x, float min_x, float max_y, float min_y)
{
	return ((x - min_x) / (max_x - min_x)) * (max_y - min_y) + min_y; 
}

Direction rollDice(DirectionProb directionProb)
{
	float dice = hf::randf(0, 1); 
	float bound = directionProb.up; 
	if (dice <= bound)
	{
		return Direction::Up;
	}
	if (dice > bound && dice <= bound + directionProb.down)
	{
		return Direction::Down; 
	}
	bound += directionProb.down;

	if (dice > bound && dice <= bound + directionProb.left)
	{
		return Direction::Left; 
	}
	bound += directionProb.left;
	if (dice > bound && dice <= bound + directionProb.right)
	{
		return Direction::Right; 
	}
	
	return Direction::None;

}

int hf::randint(int l, int u)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> distribution(l, u);

	int randomNum = distribution(gen);

	return randomNum;
}

float hf::randf(float l, float u)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> distribution(l, u);

	float randomFloat = distribution(gen);

	return randomFloat;
}

float hf::avg(float x, float y)
{
	return (x + y) / 2; 
}
