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
    // ������������ȡָ�����µ�����
    int daysInMonth(int y, int m) const;

    // ��������������Ƿ�������
    bool isLeapYear(int y) const;
    Date();
    Date(std::string& str);
    Date(int y, int m, int d);
    bool operator==(const Date& d);
    bool operator<(const Date& d);
    // ����ǰ׺++
    Date& operator++();
};

struct Coordinate {
    int num_yard;
    int x;
    int y;
    int horizontal_or_vertical; // ������� ��1��2
    Coordinate(int n = -1, int x = -1, int y = -1, int h = -1);
};


struct Yard {
    int length;
    int width;
    int cell_length;
    int cell_width;
    int distance_to_block; // �ܶ�Ӣ��Ϊblock
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
    // ����GA˳�򽻲�
    bool operator==(const Segment& b);
};

struct Result {
    int n_segments_timeout; // ���ܰ����볡�ķֶ�����
    double total_space_utilized_rate; // ����ƽ���ռ�������
    double avg_distance; // ���Ƿ���ķֶε��ܶ�ƽ������
    Result(int n_segments_timeout = 0, double total_space_utilized_rate = 0, double avg_distance = 0);
    double fitnessValue(); // ������Ӧֵ
};