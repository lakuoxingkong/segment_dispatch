#pragma once
#include<vector>
#include<list>
#include"DataStructs.h"

class FileReader
{
public:
	// ��ȡ�ѳ�����
	static std::vector<Yard> readYardData(const std::string& filename);
	// ��ȡ���볡�ֶ�����
	static std::list<Segment> readSegmentData(const std::string& filename);
	// ��ȡ�ѳ��зֶ�����
	static std::list<Segment> readSegmentInYardData(const std::string& filename);
};

