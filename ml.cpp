
#include <iostream>
#include <vector>
#include <cassert>
#include <functional>

template<typename T>
void print_container(T& container) {
	for (const auto& pc : container)
		std::cout << pc << '\t';
	std::cout << '\n';
}


template <typename T>
std::vector<T> operation_vector(std::vector<T>& first, std::vector<T> second, std::function<T(T, T)> func){
	assert(first.size() == second.size());
	std::vector<T> tmp(first.size());
	for (size_t i = 0; i < tmp.size(); ++i)
		tmp[i] = func(first[i], second[i]);
	return tmp;

}


double neural_network(std::vector<std::vector<double>>& matrix) {

	std::vector<double> tmp_vec = operation_vector<double>(matrix[0], matrix[1], [](double x, double y) {return x * y; });
	double prediction = 0;
	for (const auto& it : tmp_vec) {
		prediction += it;
	}
	return prediction;
}

template<typename T>
void print_matrix(std::vector<std::vector<T>>& matrix) {
	for (size_t width = 0; width < matrix.size(); ++width) {
		for (size_t height = 0; height < matrix[width].size(); ++height) {
			std::cout << matrix[width][height] << '\t';
		}
		std::cout << '\n';
	}
}


template<typename T>
void set_matrix(std::vector<std::vector<T>>& matrix) {
	for (size_t width = 0; width < matrix.size(); ++width) {
		for (size_t height = 0; height < matrix[width].size(); ++height) {
			matrix[width][height] = 0;
		}
	}
}


int main()
{
	unsigned width = 3;
	unsigned height = 2;
	std::vector<std::vector<double>> matrix(height);
	for (size_t i = 0; i < matrix.size(); ++i) {
		matrix[i].resize(width);
	}
	
	set_matrix(matrix);
	// 1 - string 2 - columns
	matrix[0][0] = 8;
	matrix[0][1] = 2;
	matrix[0][2] = 4;

	matrix[1][0] = 3.6;
	matrix[1][1] = 4.1;
	matrix[1][2] = 2.3;

	print_matrix(matrix);

	std::cout << neural_network(matrix) << "%" << '\n';

	

	return 0;
}
