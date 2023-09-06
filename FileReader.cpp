#include "FileReader.h"
#include<fstream>

std::vector<Yard> FileReader::readYardData(const std::string& filename)
{  
    std::ifstream file(filename); 
    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return std::vector<Yard>();
    }
    int num_yards = 0;
    file >> num_yards;
    std::vector<Yard> yards(num_yards);

    for (int i = 0; i < num_yards; ++i) {
        file >> yards[i].length >> yards[i].width >> yards[i].cell_length >> yards[i].cell_width >> yards[i].distance_to_block;
        // 将freemap初始化为0
        yards[i].free_map.assign(yards[i].length, std::vector<int>(yards[i].width, 0));
    }
    

    file.close();
    return yards;
}

std::list<Segment> FileReader::readSegmentData(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return std::list<Segment>();
    }
    std::string line;
    std::list<Segment> segments;
    // 跳过首行
    std::getline(file, line);
    while (std::getline(file, line)) {
        segments.push_back(Segment(line));
    }
    return segments;
}

std::list<Segment> FileReader::readSegmentInYardData(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return std::list<Segment>();
    }
    std::string line;
    std::list<Segment> segments;
    // 跳过首行
    std::getline(file, line);
    while (std::getline(file, line)) {
        Segment segment;
        std::istringstream tokenStream(line);
        std::string token;
        std::getline(tokenStream, token, ',');
        segment.number = std::stoi(token);
        std::getline(tokenStream, token, ',');
        segment.length = std::stoi(token);
        std::getline(tokenStream, token, ',');
        segment.width = std::stoi(token);
        std::getline(tokenStream, token, ',');
        segment.weight = std::stoi(token);
        std::getline(tokenStream, token, ',');
        segment.planning_out_time = Date(token);
        std::getline(tokenStream, token, ',');
        segment.coordinate.num_yard = std::stoi(token);
        std::getline(tokenStream, token, ',');
        segment.coordinate.x = std::stoi(token);
        std::getline(tokenStream, token, ',');
        segment.coordinate.y = std::stoi(token);
        std::getline(tokenStream, token, ',');
        segment.coordinate.horizontal_or_vertical = std::stoi(token);
    }
    return segments;
}
