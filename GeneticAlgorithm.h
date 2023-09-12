#pragma once
#include"DataStructs.h"
#include<list>

class GeneticAlgorithm
{
public:
	int n_population;
	double p_crossing;
	double p_mutation;
	int max_generatoin;
	// return×î¼Ñ½á¹û
	Result geneticAlgorithm(std::vector<Yard> yards, std::list<Segment> segments, std::list<Segment> segments_in_yard, Date today);
	GeneticAlgorithm(int np=0, double pc=0.0, double pm=0.0, int mg=0);
};

