#include "GeneticAlgorithm.h"
#include<algorithm>
#include<random>
#include"RandomAlgorithm.h"


Result GeneticAlgorithm::geneticAlgorithm(std::vector<Yard> yards, std::list<Segment> segments, std::list<Segment> segments_in_yard, Date today)
{
	
	int total_space = 0; // 各堆场总面积，按实际长度算，而不是格子数
	int space_utilized = 0; // 各堆场已利用的面积
	double total_space_utilized_rate = 0; // 多日平均空间利用率
	int total_distance = 0; // 我们放入的所有分段到总段距离之和
	int segments_origin_size = segments.size(); // 用于除total_distance
	int day = 0;
	int n_segments_timeout = 0; // 不能按期入场的分段数量
	// yards按到总段距离升序排序，便于后面的greedy算法
	std::sort(yards.begin(), yards.end(), [](const Yard& a, const Yard& b) {
		return a.distance_to_block < b.distance_to_block;
		});
	for (const auto& y : yards) { // 计算total_space
		total_space += y.length * y.width;
	}
	// 初始化freemap：遍历segments_in_yard已在场分段
	for (auto& s : segments_in_yard) {
		RandomAlgorithm::updateFreemap(s.length, s.width, yards[s.coordinate.num_yard], s.coordinate.x, s.coordinate.y, s.coordinate.horizontal_or_vertical, 1);
		space_utilized += s.length * s.width;
	}
	// 随机数种子初始化，以确保每次运行都会生成不同的随机序列
	std::srand(unsigned(std::time(0)));
	// 初始化种群
	std::vector<std::list<Segment>> population(this->n_population, std::list<Segment>(segments)); // 种群
	std::vector<std::list<Segment>> selected_population(this->n_population, std::list<Segment>(segments)); // 被选中的种群
	std::vector<double> fitness(this->n_population, 0.0); // 适应度
	std::vector<double> fitRatio(this->n_population, 0.0); // 用于轮盘赌选择
	int i = 0;
	double fitsum = 0;
	double best_fitness = - 1e8; // fitness最高的个体（调度顺序）的fitness值
	std::list<Segment> best_chrom; // fitness最高的个体（调度顺序）
	Result best_result;
	for (auto& chrom: population) {
		// 使用 std::shuffle 来随机打乱 list，以随机初始化
		// todo: 按照最早入场日期
		std::vector<Segment> temp(segments.begin(), segments.end());
		std::shuffle(temp.begin(), temp.end(), std::mt19937(std::random_device()()));
		
		chrom.assign(temp.begin(), temp.end());
		// 可考虑再按最晚入场时间从早到晚排序，相当于只打乱了最晚入场时间相同的分段的顺序
		/*chrom.sort([](Segment& a, Segment& b) {
			return a.latest_in_time < b.latest_in_time;
			});*/

		// 评估chrom：可直接调用greedy
		Result result = RandomAlgorithm::greedyAlgorithm(yards, chrom, segments_in_yard, today);
		fitness[i] = result.fitnessValue();
		fitsum += fitness[i];
		if (fitness[i] > best_fitness) {
			best_fitness = fitness[i];
			best_chrom = population[i];
			best_result = result;
		}
		++i;
	}
	// GA循环多代
	for (int generation = 0; generation < max_generatoin; ++generation) {
		// 选择
		for (int i = 0; i < n_population; ++i)	{ // 计算比例用于轮盘赌
			fitRatio[i] = fitness[i] / fitsum;
		}
		std::random_device rd;  // 随机设备，用于获取随机种子
		std::mt19937 gen(rd()); // 使用 Mersenne Twister 伪随机数生成器
		std::uniform_real_distribution<double> dist(0.0, 1.0);

		for (int j = 0; j < n_population; ++j)
		{ // 轮盘赌选择 
			double pick = dist(gen); // 0-1.0随机数
			for (int i = 0; i < n_population; ++i)
			{
				pick -= fitRatio[i];
				if (pick <= 0)
				{
					selected_population[j] = population[i];
					break;
				}
			}
		}
		// 交叉
		for (int i = 0; i <= n_population - 2; i = i + 2)
		{//交叉 相邻两条染色体 顺序交叉
			double pick = dist(gen); // 0-1.0随机数
			if (pick > this->p_crossing)
			{ // 不交叉 
				population[i] = selected_population[i];
				population[i + 1] = selected_population[i + 1];
			}
			else
			{ // 交叉
				std::vector<Segment> parent1(selected_population[i].begin(), selected_population[i].end());
				std::vector<Segment> parent2(selected_population[i + 1].begin(), selected_population[i + 1].end());
				// 随机选择两个交叉点
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> distribution(0, parent1.size() - 1);
				int point1 = distribution(gen);
				int point2 = distribution(gen);
				while (point1 == point2) {
					point2 = distribution(gen);
				}
				if (point1 > point2) {
					std::swap(point1, point2);
				}

				// 创建两个子代，初始化为-1（表示未访问的城市）
				std::vector<Segment> child1(parent1.size());
				std::vector<Segment> child2(parent2.size());

				// 复制第一个交叉点之间的城市
				copy(parent1.begin() + point1, parent1.begin() + point2 + 1, child1.begin() + point1);
				copy(parent2.begin() + point1, parent2.begin() + point2 + 1, child2.begin() + point1);

				// 填充剩余的城市
				int index1 = (point2 + 1) % parent1.size();
				int index2 = (point2 + 1) % parent2.size();

				for (const Segment& city : parent2) {
					if (find(child1.begin(), child1.end(), city) == child1.end()) {
						child1[index1] = city;
						index1 = (index1 + 1) % parent1.size();
					}
				}

				for (const Segment& city : parent1) {
					if (find(child2.begin(), child2.end(), city) == child2.end()) {
						child2[index2] = city; 
						index2 = (index2 + 1) % parent2.size();
					}
				}
				selected_population[i].assign(child1.begin(), child1.end());
				selected_population[i + 1].assign(child2.begin(), child2.end());
			}
		}
		// 变异 采用交换变异
		for (int i = 0; i < n_population; ++i)
		{// 变异  
			double pick = dist(gen);
			if (pick <= p_mutation)
			{
				// todo：限制只能同一天的交换
				std::uniform_int_distribution<int> dist2(0, segments.size() - 1);
				int mutation_point1 = dist2(gen);
				int mutation_point2 = dist2(gen);
				auto it1 = std::next(selected_population[i].begin(), mutation_point1); // 变异的第1个元素
				auto it2 = std::next(selected_population[i].begin(), mutation_point2); // 变异的第2个元素

				std::iter_swap(it1, it2); // 交换两个元素
			}

		}
		// 将进化后种群赋值给population
		for (int i = 0; i < population.size(); ++i) {
			population[i] = selected_population[i];
		}
		// 再评估
		fitsum = 0;
		for (int i = 0; i < n_population; ++i) {
			// 评估chrom：可直接调用greedy
			Result result = RandomAlgorithm::greedyAlgorithm(yards, population[i], segments_in_yard, today);
			fitness[i] = result.fitnessValue();
			fitsum += fitness[i];
			if (fitness[i] > best_fitness) {
				best_fitness = fitness[i];
				best_chrom = population[i];
				best_result = result;
			}
		}
	}
	printf("最佳结果：\n适应值:%f\n不能按期入场的分段数量:%d\n多日平均空间利用率:%f\n我们放入的分段到总段平均距离:%f\n",
		best_result.fitnessValue(), best_result.n_segments_timeout, best_result.total_space_utilized_rate, best_result.avg_distance);
	return best_result;
}

GeneticAlgorithm::GeneticAlgorithm(int np, double pc, double pm, int mg):
	n_population(np), p_crossing(pc), p_mutation(pm), max_generatoin(mg)
{
}
