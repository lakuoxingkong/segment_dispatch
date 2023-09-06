#include "RandomAlgorithm.h"
#include<random>

// 此函数内不考虑横放或竖放
bool isSpaceAvailable(int length, int width, const Yard& yard, int x, int y) {
	// todo:边界可以再处理，浮点数？
	// 检查越界
	if (x + length / yard.cell_length >= yard.free_map.size() || y + width / yard.cell_width >= yard.free_map[0].size()) {
		return false;
	}
	for (int i = x; i <= x + length / yard.cell_length; ++i) {
		for (int j = y; j <= y + width / yard.cell_width; ++j) {
			// 检查每小格是否为空
			if (yard.free_map[i][j] == 1) {
				return false;
			}
		}
	}
	return true;
}

// horizontal_or_vertical: 1 for horizontal, 2 for vertical
// in_or_out: 1 for in, 2 for out
void updateFreemap(int length, int width, Yard& yard, int x, int y, int horizontal_or_vertical, int in_or_out) {
	// 默认当他不越界
	if (horizontal_or_vertical == 1) {
		// todo:边界可以再处理，浮点数？
		for (int i = x; i <= x + length / yard.cell_length; ++i) {
			for (int j = y; j <= y + width / yard.cell_width; ++j) {
				if (in_or_out == 1) {
					yard.free_map[i][j] = 1;
				}
				else {
					yard.free_map[i][j] = 0;
				}
			}
			//return (length / yard.cell_length + 1) * (width / yard.cell_width + 1);
		}
	}
	else {
		for (int i = x; i <= x + width / yard.cell_length; ++i) {
			for (int j = y; j <= y + length / yard.cell_width; ++j) {
				if (in_or_out == 1) {
					yard.free_map[i][j] = 1;
				}
				else {
					yard.free_map[i][j] = 0;
				}
			}
			//return (width / yard.cell_length + 1) * (length / yard.cell_width + 1);
		}
	}
	//return 0;
}

void RandomAlgorithm::randomAlgorithm(std::vector<Yard> yards, std::list<Segment> segments, std::list<Segment> segments_in_yard, Date today)
{
	int total_space = 0; // 各堆场总面积，按实际长度算，而不是格子数
	int space_utilized = 0; // 各堆场已利用的面积
	double total_space_utilized_rate = 0; // 多日平均空间利用率
	int total_distance = 0; // 我们放入的所有分段到总段距离之和
	int segments_origin_size = segments.size(); // 用于除total_distance
	int day = 0;
	int n_segments_timeout = 0; // 不能按期入场的分段数量
	for (const auto& y : yards) { // 计算total_space
		total_space += y.length * y.width;
	}
	// 初始化freemap：遍历segments_in_yard已在场分段
	for (auto& s : segments_in_yard) {
		updateFreemap(s.length, s.width, yards[s.coordinate.num_yard], s.coordinate.x, s.coordinate.y, s.coordinate.horizontal_or_vertical, 1);
		space_utilized += s.length * s.width;
	}
	std::list<Segment> segments_to_enter;
	// 待入场分段未全部入场，则一直循环
	while (!segments.empty() || !segments_to_enter.empty()) {
		// todo：可考虑先按日期排序
		// 先出场：遍历segments_in_yard，更新freemap
		for (auto it = segments_in_yard.begin(); it != segments_in_yard.end(); /* 注意：不自动递增 */) {
			if (it->planning_out_time == today) {
				// 计划出场就是今天，出场，更新freemap
				updateFreemap(it->length, it->width, yards[it->coordinate.num_yard],
					it->coordinate.x, it->coordinate.y, it->coordinate.horizontal_or_vertical, 2);
				space_utilized -= it->length * it->width;
				// 使用迭代器删除元素，并将迭代器更新为下一个元素
				it = segments_in_yard.erase(it);
			}
			else {
				// 如果不删除元素，手动递增迭代器
				++it;
			}
		}

		// 后入场：从segments中选取当日入场，转入segments_to_enter数组
		for (auto it = segments.begin(); it != segments.end(); /* 注意：不自动递增 */) {
			if (it->planning_in_time == today) {
				// 今日入场的放入segments_to_enter
				segments_to_enter.push_back(*it);
				// 使用迭代器删除元素，并将迭代器更新为下一个元素
				it = segments.erase(it);
			}
			else {
				// 如果不删除元素，手动递增迭代器
				++it;
			}
		}
		// 按固定顺序(todo:按随机顺序?)选计划当天入场的，随机选取（有空的）摆放位置
		for (auto it = segments_to_enter.begin(); it != segments_to_enter.end(); /* 注意：不自动递增 */) {
			Segment& s = *it;
			// 从可行的位置中随机选
			// 先找出可行位置：用左下角格子坐标表示
			std::vector<Coordinate> valid_coordinates;
			// 遍历堆场找出所有可行位置
			for (int n = 0; n < yards.size(); ++n) {
				Yard y = yards[n];
				// 遍历堆场每格
				for (int i = 0; i < y.free_map.size(); ++i) {
					for (int j = 0; j < y.free_map[0].size(); ++j) {
						// 检查每格是否放得下
						if (y.free_map[i][j] == 0) {
							// 横放
							if (isSpaceAvailable(s.length, s.width, y, i, j)) {
								valid_coordinates.push_back(Coordinate(n, i, j, 1));
							}
							// 竖放
							if (isSpaceAvailable(s.width, s.length, y, i, j)) {
								valid_coordinates.push_back(Coordinate(n, i, j, 2));
							}
						}
					}
				}
			}
			 // 若该分段已无位置可放
			if (valid_coordinates.empty()) {
				if (s.planning_in_time == s.latest_in_time) {
					// todo: 过了最后期限？
					++n_segments_timeout;
					it = segments_to_enter.erase(it);
				}
				else {
					// 推迟到第二天
					++s.planning_in_time;
					++it;
				}
			}
			else {
				// 从valid_coordinates中随机选一个位置放分段
				// 创建随机数生成器引擎
				std::random_device rd;  // 随机设备，用于获取随机种子
				std::mt19937 gen(rd()); // 使用 Mersenne Twister 伪随机数生成器
				// 定义随机数分布范围
				int lower_bound = 1;   // 最小值
				int upper_bound = valid_coordinates.size(); // 最大值
				std::uniform_int_distribution<int> dist(lower_bound, upper_bound);
				// 生成随机数
				int random_number = dist(gen);

				s.coordinate = valid_coordinates[random_number - 1];
				// update freemap
				updateFreemap(s.length, s.width, yards[s.coordinate.num_yard],
					s.coordinate.x, s.coordinate.y, s.coordinate.horizontal_or_vertical, 1);
				
				space_utilized += s.length * s.width; // 累加利用面积
				total_distance += yards[it->coordinate.num_yard].distance_to_block;
				segments_in_yard.push_back(s);
				it = segments_to_enter.erase(it);
			}	
		}
		double today_space_utilized_rate = (double)space_utilized / total_space;
		total_space_utilized_rate += today_space_utilized_rate;
		++day; // 日数+1
		++today; // 日期+1
	}

	total_space_utilized_rate /= day; // 多日平均空间利用率

	printf("不能按期入场的分段数量:%d\n多日平均空间利用率:%f\n我们放入的分段到总段平均距离:%f\n", 
		n_segments_timeout, total_space_utilized_rate, (double)total_distance / segments_origin_size);
	printf("%d %d\n", day, total_distance);

}


