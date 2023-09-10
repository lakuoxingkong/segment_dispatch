#include "GeneticAlgorithm.h"
#include<algorithm>
#include<random>
#include"RandomAlgorithm.h"


void GeneticAlgorithm::geneticAlgorithm(std::vector<Yard> yards, std::list<Segment> segments, std::list<Segment> segments_in_yard, Date today)
{
	puts("---geneticAlgorithm---");
	int total_space = 0; // ���ѳ����������ʵ�ʳ����㣬�����Ǹ�����
	int space_utilized = 0; // ���ѳ������õ����
	double total_space_utilized_rate = 0; // ����ƽ���ռ�������
	int total_distance = 0; // ���Ƿ�������зֶε��ܶξ���֮��
	int segments_origin_size = segments.size(); // ���ڳ�total_distance
	int day = 0;
	int n_segments_timeout = 0; // ���ܰ����볡�ķֶ�����
	// yards�����ܶξ����������򣬱��ں����greedy�㷨
	std::sort(yards.begin(), yards.end(), [](const Yard& a, const Yard& b) {
		return a.distance_to_block < b.distance_to_block;
		});
	for (const auto& y : yards) { // ����total_space
		total_space += y.length * y.width;
	}
	// ��ʼ��freemap������segments_in_yard���ڳ��ֶ�
	for (auto& s : segments_in_yard) {
		RandomAlgorithm::updateFreemap(s.length, s.width, yards[s.coordinate.num_yard], s.coordinate.x, s.coordinate.y, s.coordinate.horizontal_or_vertical, 1);
		space_utilized += s.length * s.width;
	}
	// ��������ӳ�ʼ������ȷ��ÿ�����ж������ɲ�ͬ���������
	std::srand(unsigned(std::time(0)));
	// ��ʼ����Ⱥ
	std::vector<std::list<Segment>> population(this->n_population, std::list<Segment>(segments)); // ��Ⱥ
	std::vector<std::list<Segment>> selected_population(this->n_population, std::list<Segment>(segments)); // ��ѡ�е���Ⱥ
	std::vector<double> fitness(this->n_population, 0.0); // ��Ӧ��
	std::vector<double> fitRatio(this->n_population, 0.0); // �������̶�ѡ��
	int i = 0;
	double fitsum = 0;
	int best_fitness = 0; // fitness��ߵĸ��壨����˳�򣩵�fitnessֵ
	std::list<Segment> best_chrom; // fitness��ߵĸ��壨����˳��
	Result best_result;
	for (auto& chrom: population) {
		// ʹ�� std::shuffle ��������� list���������ʼ��
		// todo: ���������볡����
		std::vector<Segment> temp(segments.begin(), segments.end());
		std::shuffle(temp.begin(), temp.end(), std::mt19937(std::random_device()()));

		chrom.assign(temp.begin(), temp.end());
		printf("%d %d\n", segments.begin()->number, chrom.begin()->number);

		// ����chrom����ֱ�ӵ���greedy
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
	// GAѭ�����
	for (int generation = 0; generation < max_generatoin; ++generation) {
		// ѡ��
		for (int i = 0; i < n_population; ++i)	{ // ��������������̶�
			fitRatio[i] = fitness[i] / fitsum;
		}
		std::random_device rd;  // ����豸�����ڻ�ȡ�������
		std::mt19937 gen(rd()); // ʹ�� Mersenne Twister α�����������
		std::uniform_real_distribution<double> dist(0.0, 1.0);

		for (int j = 0; j < n_population; ++j)
		{ // ���̶�ѡ�� 
			double pick = dist(gen); // 0-1.0�����
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
		// ����
		for (int i = 0; i <= n_population - 2; i = i + 2)
		{//���� ��������Ⱦɫ�� ���㽻��
			double pick = dist(gen); // 0-1.0�����
			if (pick > this->p_crossing)
			{ // ������ 
				population[i] = selected_population[i];
				population[i + 1] = selected_population[i + 1];
			}
			else
			{ // ����
				std::uniform_int_distribution<int> dist2(1, segments.size() - 1);
				int crossing_point = dist2(gen);
				// ʹ�� std::advance �ƶ���������ָ��λ��
				auto it1 = selected_population[i].begin();
				auto it2 = selected_population[i + 1].begin();
				std::advance(it1, crossing_point);
				std::advance(it2, crossing_point);

				while (it1 != selected_population[i].end() && it2 != selected_population[i + 1].end()) {
					std::swap(*it1, *it2);
					++it1;
					++it2;
				}
			}
		}
		// ���� ���ý�������
		for (int i = 0; i < n_population; ++i)
		{// ����  
			double pick = dist(gen);
			if (pick <= p_mutation)
			{
				// todo������ֻ��ͬһ��Ľ���
				std::uniform_int_distribution<int> dist2(0, segments.size() - 1);
				int mutation_point1 = dist2(gen);
				int mutation_point2 = dist2(gen);
				auto it1 = std::next(selected_population[i].begin(), mutation_point1); // ����ĵ�1��Ԫ��
				auto it2 = std::next(selected_population[i].begin(), mutation_point2); // ����ĵ�2��Ԫ��

				std::iter_swap(it1, it2); // ��������Ԫ��
			}

		}
		// ����������Ⱥ��ֵ��population
		for (int i = 0; i < population.size(); ++i) {
			population[i] = selected_population[i];
		}
		// ������
		fitsum = 0;
		for (int i = 0; i < n_population; ++i) {
			// ����chrom����ֱ�ӵ���greedy
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
	printf("��ѽ����\n���ܰ����볡�ķֶ�����:%d\n����ƽ���ռ�������:%f\n���Ƿ���ķֶε��ܶ�ƽ������:%f\n",
		best_result.n_segments_timeout, best_result.total_space_utilized_rate, best_result.avg_distance);

}

GeneticAlgorithm::GeneticAlgorithm(int np, double pc, double pm, int mg):
	n_population(np), p_crossing(pc), p_mutation(pm), max_generatoin(mg)
{
}
