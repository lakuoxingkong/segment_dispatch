#pragma once
#include<string>
#include<sstream>
#include<iostream>
#include<vector>

class Date {
public:
    int year;
    int month;
    int day;
    // 辅助函数：获取指定年月的天数
    int daysInMonth(int y, int m) const;

    // 辅助函数：检查是否是闰年
    bool isLeapYear(int y) const;
    Date();
    Date(std::string& str);
    Date(int y, int m, int d);
    bool operator==(const Date& d);
    bool operator<(const Date& d);
    // 重载前缀++
    Date& operator++();
};

struct Coordinate {
    int num_yard;
    int x;
    int y;
    int horizontal_or_vertical; // 横或竖， 横1竖2
    Coordinate(int n = -1, int x = -1, int y = -1, int h = -1);
};


struct Yard {
    int length;
    int width;
    int cell_length;
    int cell_width;
    int distance_to_block; // 总段英文为block
    std::vector<std::vector<int>> free_map;
    Yard();
};

struct Segment {
    int number;
    int length;
    int width;
    int weight;
    Date earliest_in_time;
    Date latest_in_time;
    Date planning_in_time;
    Date planning_out_time;
    Coordinate coordinate;
    Segment();
    Segment(std::string& str);
    // 用于GA顺序交叉
    bool operator==(const Segment& b);
};

struct Result {
    int n_segments_timeout; // 不能按期入场的分段数量
    double total_space_utilized_rate; // 多日平均空间利用率
    double avg_distance; // 我们放入的分段到总段平均距离
    Result(int n_segments_timeout = 0, double total_space_utilized_rate = 0, double avg_distance = 0);
    double fitnessValue(); // 计算适应值
};