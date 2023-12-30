#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <random>
#include <time.h>
#include <cmath>
#include <Windows.h>

namespace hf
{
	inline int randint(int l, int u);
	inline float randf(float l, float u);
	float avg(float x, float y);
	float interpolate(float x, float max_x, float min_x, float max_y, float min_y);
};

struct Coords
{
	float x;
	float y;
	Coords() { }
	Coords(float _x, float _y) : x(_x), y(_y) {}
	static Coords randomize(float width, float height)
	{
		return Coords(hf::randf(0, width), hf::randf(0, height));
	}
	inline Coords operator + (const float& val)
	{
		return { x + val, y + val };
	}
	inline void operator = (const Coords& n)
	{
		x = n.x;
		y = n.y;
	}
};
enum class Direction {
	None, Up, Down, Left, Right
};
struct DirectionProb
{
	float up;
	float down;
	float left;
	float right;
	DirectionProb() : up(0.25f), down(0.25f), left(0.25f), right(0.25f) {}

	DirectionProb(float u, float d, float l, float r) : up(u), down(d), left(l), right(r)
	{
		float extra = std::fmod((up + left + right + down), 1.0) / 4.0f;
		up -= extra;
		down -= extra;
		left -= extra;
		right -= extra;
	}
	inline void operator = (const DirectionProb& n)
	{
		up = n.up;
		down = n.down;
		left = n.left;
		right = n.right;
	}

};
struct Dimension
{
	float width;
	float height;
};

//std::pair<std::pair<float, float>, std::pair<float, float>> hash(float x, float y, float radius);
Direction rollDice(DirectionProb directionProb);

