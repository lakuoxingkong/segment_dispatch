#include "DataStructs.h"

Coordinate::Coordinate(int n, int x, int y, int h)
{
	this->num_yard = n;
	this->x = x;
	this->y = y;
    this->horizontal_or_vertical = h;
}

Segment::Segment()
{

}

Segment::Segment(std::string& str)
{
    std::istringstream tokenStream(str);
    std::string token;
    std::getline(tokenStream, token, ',');
    this->number = std::stoi(token);
    std::getline(tokenStream, token, ',');
    this->length = std::stoi(token);
    std::getline(tokenStream, token, ',');
    this->width = std::stoi(token);
    std::getline(tokenStream, token, ',');
    this->weight = std::stoi(token);
    std::getline(tokenStream, token, ',');
    this->earliest_in_time = Date(token);
    this->planning_in_time = earliest_in_time;
    std::getline(tokenStream, token, ',');
    this->latest_in_time = Date(token);
    std::getline(tokenStream, token, ',');
    this->planning_out_time = Date(token);
} 


Yard::Yard(): free_map()
{
	length = 0; width = 0; cell_length = 0; cell_width = 0; distance_to_block = 0;
}

int Date::daysInMonth(int y, int m) const {
    static const int daysPerMonth[] = {
        0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
    int days = daysPerMonth[m];
    if (m == 2 && isLeapYear(y)) {
        days++;  // 闰年的2月多一天
    }
    return days;
}

bool Date::isLeapYear(int y) const {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

Date::Date() { year = 0; month = 0; day = 0; }

Date::Date(std::string& str) {
    std::istringstream tokenStream(str);
    std::string token;
    std::getline(tokenStream, token, '/');
    this->year = std::stoi(token);
    std::getline(tokenStream, token, '/');
    this->month = std::stoi(token);
    std::getline(tokenStream, token, '/');
    this->day = std::stoi(token);
}

Date::Date(int y, int m, int d)
{
    year = y; month = m; day = d;
}

bool Date::operator==(const Date& d)
{
    return this->year == d.year && this->day == d.day && this->month == d.month;
}

Date& Date::operator++() {
    ++day;
    // 处理溢出和月份变化
    if (day > daysInMonth(year, month)) {
        day = 1;
        month++;

        if (month > 12) {
            month = 1;
            year++;
        }
    }
    return *this;  // 返回自身以支持连续的++操作
}
