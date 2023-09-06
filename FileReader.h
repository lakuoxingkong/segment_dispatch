#pragma once
#include<vector>
#include<list>
#include"DataStructs.h"

class FileReader
{
public:
	// 读取堆场数据
	static std::vector<Yard> readYardData(const std::string& filename);
	// 读取待入场分段数据
	static std::list<Segment> readSegmentData(const std::string& filename);
	// 读取堆场中分段数据
	static std::list<Segment> readSegmentInYardData(const std::string& filename);
};

