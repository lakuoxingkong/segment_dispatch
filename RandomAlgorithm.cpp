#include "RandomAlgorithm.h"
#include<random>

// �˺����ڲ����Ǻ�Ż�����
bool isSpaceAvailable(int length, int width, const Yard& yard, int x, int y) {
	// todo:�߽�����ٴ�����������
	// ���Խ��
	if (x + length / yard.cell_length >= yard.free_map.size() || y + width / yard.cell_width >= yard.free_map[0].size()) {
		return false;
	}
	for (int i = x; i <= x + length / yard.cell_length; ++i) {
		for (int j = y; j <= y + width / yard.cell_width; ++j) {
			// ���ÿС���Ƿ�Ϊ��
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
	// Ĭ�ϵ�����Խ��
	if (horizontal_or_vertical == 1) {
		// todo:�߽�����ٴ�����������
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
	int total_space = 0; // ���ѳ����������ʵ�ʳ����㣬�����Ǹ�����
	int space_utilized = 0; // ���ѳ������õ����
	double total_space_utilized_rate = 0; // ����ƽ���ռ�������
	int total_distance = 0; // ���Ƿ�������зֶε��ܶξ���֮��
	int segments_origin_size = segments.size(); // ���ڳ�total_distance
	int day = 0;
	int n_segments_timeout = 0; // ���ܰ����볡�ķֶ�����
	for (const auto& y : yards) { // ����total_space
		total_space += y.length * y.width;
	}
	// ��ʼ��freemap������segments_in_yard���ڳ��ֶ�
	for (auto& s : segments_in_yard) {
		updateFreemap(s.length, s.width, yards[s.coordinate.num_yard], s.coordinate.x, s.coordinate.y, s.coordinate.horizontal_or_vertical, 1);
		space_utilized += s.length * s.width;
	}
	std::list<Segment> segments_to_enter;
	// ���볡�ֶ�δȫ���볡����һֱѭ��
	while (!segments.empty() || !segments_to_enter.empty()) {
		// todo���ɿ����Ȱ���������
		// �ȳ���������segments_in_yard������freemap
		for (auto it = segments_in_yard.begin(); it != segments_in_yard.end(); /* ע�⣺���Զ����� */) {
			if (it->planning_out_time == today) {
				// �ƻ��������ǽ��죬����������freemap
				updateFreemap(it->length, it->width, yards[it->coordinate.num_yard],
					it->coordinate.x, it->coordinate.y, it->coordinate.horizontal_or_vertical, 2);
				space_utilized -= it->length * it->width;
				// ʹ�õ�����ɾ��Ԫ�أ���������������Ϊ��һ��Ԫ��
				it = segments_in_yard.erase(it);
			}
			else {
				// �����ɾ��Ԫ�أ��ֶ�����������
				++it;
			}
		}

		// ���볡����segments��ѡȡ�����볡��ת��segments_to_enter����
		for (auto it = segments.begin(); it != segments.end(); /* ע�⣺���Զ����� */) {
			if (it->planning_in_time == today) {
				// �����볡�ķ���segments_to_enter
				segments_to_enter.push_back(*it);
				// ʹ�õ�����ɾ��Ԫ�أ���������������Ϊ��һ��Ԫ��
				it = segments.erase(it);
			}
			else {
				// �����ɾ��Ԫ�أ��ֶ�����������
				++it;
			}
		}
		// ���̶�˳��(todo:�����˳��?)ѡ�ƻ������볡�ģ����ѡȡ���пյģ��ڷ�λ��
		for (auto it = segments_to_enter.begin(); it != segments_to_enter.end(); /* ע�⣺���Զ����� */) {
			Segment& s = *it;
			// �ӿ��е�λ�������ѡ
			// ���ҳ�����λ�ã������½Ǹ��������ʾ
			std::vector<Coordinate> valid_coordinates;
			// �����ѳ��ҳ����п���λ��
			for (int n = 0; n < yards.size(); ++n) {
				Yard y = yards[n];
				// �����ѳ�ÿ��
				for (int i = 0; i < y.free_map.size(); ++i) {
					for (int j = 0; j < y.free_map[0].size(); ++j) {
						// ���ÿ���Ƿ�ŵ���
						if (y.free_map[i][j] == 0) {
							// ���
							if (isSpaceAvailable(s.length, s.width, y, i, j)) {
								valid_coordinates.push_back(Coordinate(n, i, j, 1));
							}
							// ����
							if (isSpaceAvailable(s.width, s.length, y, i, j)) {
								valid_coordinates.push_back(Coordinate(n, i, j, 2));
							}
						}
					}
				}
			}
			 // ���÷ֶ�����λ�ÿɷ�
			if (valid_coordinates.empty()) {
				if (s.planning_in_time == s.latest_in_time) {
					// todo: ����������ޣ�
					++n_segments_timeout;
					it = segments_to_enter.erase(it);
				}
				else {
					// �Ƴٵ��ڶ���
					++s.planning_in_time;
					++it;
				}
			}
			else {
				// ��valid_coordinates�����ѡһ��λ�÷ŷֶ�
				// �������������������
				std::random_device rd;  // ����豸�����ڻ�ȡ�������
				std::mt19937 gen(rd()); // ʹ�� Mersenne Twister α�����������
				// ����������ֲ���Χ
				int lower_bound = 1;   // ��Сֵ
				int upper_bound = valid_coordinates.size(); // ���ֵ
				std::uniform_int_distribution<int> dist(lower_bound, upper_bound);
				// ���������
				int random_number = dist(gen);

				s.coordinate = valid_coordinates[random_number - 1];
				// update freemap
				updateFreemap(s.length, s.width, yards[s.coordinate.num_yard],
					s.coordinate.x, s.coordinate.y, s.coordinate.horizontal_or_vertical, 1);
				
				space_utilized += s.length * s.width; // �ۼ��������
				total_distance += yards[it->coordinate.num_yard].distance_to_block;
				segments_in_yard.push_back(s);
				it = segments_to_enter.erase(it);
			}	
		}
		double today_space_utilized_rate = (double)space_utilized / total_space;
		total_space_utilized_rate += today_space_utilized_rate;
		++day; // ����+1
		++today; // ����+1
	}

	total_space_utilized_rate /= day; // ����ƽ���ռ�������

	printf("���ܰ����볡�ķֶ�����:%d\n����ƽ���ռ�������:%f\n���Ƿ���ķֶε��ܶ�ƽ������:%f\n", 
		n_segments_timeout, total_space_utilized_rate, (double)total_distance / segments_origin_size);
	printf("%d %d\n", day, total_distance);

}


