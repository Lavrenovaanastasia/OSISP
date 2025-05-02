#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>

void sort_part(std::vector<int>& data, size_t start, size_t end) {
    std::sort(data.begin() + start, data.begin() + end);
}

std::vector<int> merge_sorted_parts(const std::vector<std::vector<int>>& parts) {
    std::vector<int> result;
    std::vector<size_t> indices(parts.size(), 0);
    for (auto p : parts)
	{
		std::cout << "Part: ";
	 for(auto p_i : p)
	 {
		 std::cout << p_i << ", " ;
	 }
	 std::cout << std::endl;
	}	    
    
    while (true) {
        int min_value = std::numeric_limits<int>::max();
        int min_index = -1;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (indices[i] < parts[i].size() && parts[i][indices[i]] < min_value) {
                min_value = parts[i][indices[i]];
                min_index = i;
            }
        }
        if (min_index == -1) break;
        result.push_back(min_value);
        indices[min_index]++;
    }
    return result;
}

int main() {
    size_t array_size, num_threads;
    std::cout << "Введите размер массива: ";
    std::cin >> array_size;
    std::cout << "Введите количество потоков: ";
    std::cin >> num_threads;

    std::vector<int> data(array_size);
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 10000);
    for (auto& val : data) val = dist(gen);
    std::cout << "Исходдный массив: ";
    for (auto &val : data) std::cout << val << ", ";
    std::cout << std::endl;
    
    std::vector<std::vector<int>> parts(num_threads);
    size_t chunk_size = array_size / num_threads;
    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? array_size : (start + chunk_size);
        parts[i] = std::vector<int>(data.begin() + start, data.begin() + end);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back(sort_part, std::ref(parts[i]), 0, parts[i].size());
    }
    for (auto& thread : threads) thread.join();
    
    std::vector<int> sorted_data = merge_sorted_parts(parts);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "END RESULT: ";
    for (auto i : sorted_data)
    {
        std::cout << i << ", " ;
    }
    std::cout << std::endl;
    
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Время выполнения: " << elapsed.count() << " секунд" << std::endl;
    return 0;
}
