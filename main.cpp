#include<stdio.h>
#include<iostream>
#include<algorithm>
#include<random>
#include"FileReader.h"
#include"RandomAlgorithm.h"
#include"GeneticAlgorithm.h"

int main() {
    std::vector<Yard> yards = FileReader::readYardData("yard_data.txt");
    std::list<Segment> segments = FileReader::readSegmentData("segment_data.csv");
    std::list<Segment> segments_in_yard = FileReader::readSegmentInYardData("segment_in_yard.csv");

    int op = 1; // 打乱顺序1，按最晚入场时间排序2
    int n_population = 20;
    int max_generatoin = 20;
    int loops = n_population * max_generatoin;
    Date today(2014, 8, 14);
    puts("---randomAlgorithm---");
    Result best_result;
    double best_fitness = - 1e8;
    for (int i = 1; i <= loops; ++i) {
        if (op == 1) {
            // 打乱segment入场顺序以重复试验
            std::vector<Segment> temp(segments.begin(), segments.end());
            std::shuffle(temp.begin(), temp.end(), std::mt19937(std::random_device()()));
            segments.assign(temp.begin(), temp.end());
        }
        else if (op == 2) {
            // 按最晚入场时间排序
            segments.sort([](Segment& a, Segment& b) {
                return a.latest_in_time < b.latest_in_time;
                });
        }
        
        Result random_result = RandomAlgorithm::randomAlgorithm(yards, segments, segments_in_yard, today);
        double fitness = random_result.fitnessValue();
        if (fitness > best_fitness) {
            best_fitness = fitness;
            best_result = random_result;
        }
    }
    printf("最佳结果：\n适应值:%f\n不能按期入场的分段数量:%d\n多日平均空间利用率:%f\n我们放入的分段到总段平均距离:%f\n",
        best_result.fitnessValue(), best_result.n_segments_timeout, best_result.total_space_utilized_rate, best_result.avg_distance);

    puts("---greedyAlgorithm---");
    best_fitness = - 1e8;
    for (int i = 1; i <= loops; ++i) {
        if (op == 1) {
            // 打乱segment入场顺序以重复试验
            std::vector<Segment> temp(segments.begin(), segments.end());
            std::shuffle(temp.begin(), temp.end(), std::mt19937(std::random_device()()));
            segments.assign(temp.begin(), temp.end());
        }
        else if (op == 2) {
            segments.sort([](Segment& a, Segment& b) {
                return a.latest_in_time < b.latest_in_time;
                });
        }
        Result greedy_result = RandomAlgorithm::greedyAlgorithm(yards, segments, segments_in_yard, today);
        double fitness = greedy_result.fitnessValue();
        if (fitness > best_fitness) {
            best_fitness = fitness;
            best_result = greedy_result;
        }
    }
    printf("最佳结果：\n适应值:%f\n不能按期入场的分段数量:%d\n多日平均空间利用率:%f\n我们放入的分段到总段平均距离:%f\n",
        best_result.fitnessValue(), best_result.n_segments_timeout, best_result.total_space_utilized_rate, best_result.avg_distance);

    puts("---geneticAlgorithm---");
    GeneticAlgorithm ga(n_population, 0.7, 0.1, max_generatoin);
    ga.geneticAlgorithm(yards, segments, segments_in_yard, today);

	puts("success.");
}