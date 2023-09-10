#pragma once
#include"DataStructs.h"
#include<vector>
#include<list>

class RandomAlgorithm
{
public:
	// 此函数内不考虑横放或竖放
	static bool isSpaceAvailable(int length, int width, const Yard& yard, int x, int y);

	// horizontal_or_vertical: 1 for horizontal, 2 for vertical
	// in_or_out: 1 for in, 2 for out
	static void updateFreemap(int length, int width, Yard& yard, int x, int y, int horizontal_or_vertical, int in_or_out);

	static void randomAlgorithm(std::vector<Yard> yards, std::list<Segment> segments, std::list<Segment> segments_in_yard, Date today);

	static Result greedyAlgorithm(std::vector<Yard> yards, std::list<Segment> segments, std::list<Segment> segments_in_yard, Date today);

};

