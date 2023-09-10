#include<stdio.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include"FileReader.h"
#include"RandomAlgorithm.h"
#include"GeneticAlgorithm.h"

int main() {
    std::vector<Yard> yards = FileReader::readYardData("yard_data.txt");
    std::list<Segment> segments = FileReader::readSegmentData("segment_data.csv");
    std::list<Segment> segments_in_yard = FileReader::readSegmentInYardData("segment_in_yard.csv");

    Date today(2014, 8, 14);
    RandomAlgorithm::randomAlgorithm(yards, segments, segments_in_yard, today);

    RandomAlgorithm::greedyAlgorithm(yards, segments, segments_in_yard, today);

    GeneticAlgorithm ga(10, 0.7, 0.1, 10);
    ga.geneticAlgorithm(yards, segments, segments_in_yard, today);

	puts("success.");
}