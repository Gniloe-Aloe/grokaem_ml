#include <vector>
#include <any>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <cmath>
#include <functional>
#include <algorithm>
#include <numeric>
template<typename T>
class My_matrix {
public:
	My_matrix() {
		height = width = 0;
	}
	//конструктор копирования от другой матрицы
	My_matrix(const My_matrix& another_matrix) {
		/*height = another_matrix.height;
		width = another_matrix.width;
		matrix = another_matrix.matrix;*/
		*this = another_matrix;
	}
	const My_matrix& operator =(const My_matrix& another_matrix) {
		height = another_matrix.height;
		width = another_matrix.width;
		matrix = another_matrix.matrix;
		return *this;
	}
	//коструктор с высотой и шириной матрицы
	My_matrix(const size_t& height, const size_t& width) {
		resize(height, width);
	}
	void resize(const size_t& height, const size_t& width) {
		this->height = height;
		this->width = width;
		matrix.resize(height);
		for (size_t height_i = 0; height_i < height; ++height_i)
			matrix[height_i].resize(width);
	}
	//перебрать все числа и применить на них функциональный объект
	void go_over_elements(std::function<void(T&)> func) {
		for (size_t height_i = 0; height_i < height; ++height_i) {
			for (size_t width_i = 0; width_i < width; ++width_i) {
				func(matrix[height_i][width_i]);
			}
		}
	}
	void print()noexcept {
		for (size_t height_i = 0; height_i < height; ++height_i) {
			for (size_t width_i = 0; width_i < width; ++width_i) {
				std::cout << matrix[height_i][width_i] << '\t';
			}
			std::cout << '\n';
		}
		std::cout << '\n';
	}
	const My_matrix operator+(const My_matrix& another_matrix) {
		My_matrix<T> tmp_matrix = *this;
		for (size_t height_i = 0; height_i < height; ++height_i) {
			for (size_t width_i = 0; width_i < width; ++width_i) {
				tmp_matrix.matrix[height_i][width_i] += another_matrix.matrix[height_i][width_i];
			}
		}
		return tmp_matrix;
	}
	const My_matrix operator*(const My_matrix& another_matrix) {
		My_matrix<T> tmp_matrix = *this;
		for (size_t height_i = 0; height_i < height; ++height_i) {
			for (size_t width_i = 0; width_i < width; ++width_i) {
				tmp_matrix.matrix[height_i][width_i] *= another_matrix.matrix[height_i][width_i];
			}
		}
		return tmp_matrix;
	}
	size_t height;
	size_t width;
	std::vector<std::vector<T>> matrix;
};

//goal_pred - верный прогноз
void neural_network(double& weight, const double& goal_pred, const double& input) {
	double pred; //прогноз
	double error;
	double derivative;//производная показывает направление
	double alpha = 0.1;//альфа-коэффициент для уменьшения производной
	for (size_t i = 0; i < 20; ++i) {
		pred = weight * input;
		error = std::pow(pred - goal_pred, 2);//квадратичная ошибка всегда положительна
		//производная положительна, если предположение больше нужного
		//и отрицательна, если предположение меньше нужного
		derivative = input * (pred - goal_pred);
		weight -= derivative * alpha;
		std::cout << i << " Error " << error << "\tPrediction " << pred << "\tWeight " << weight<< pred << "\tderivative " << derivative << "\n";
	}
}

//goal - матрица верных значений(идти/не идти), data - матрица огней светофора 
void streerlights_neunet(std::vector<double>& weights, const My_matrix<double>& goal, const My_matrix<double>& data) {
	double alpha = 0.01;
	
	std::vector<double> pred_vec = { 0, 0, 0 };//прогноз
	for (size_t learning_iteration = 0; learning_iteration < 10000; ++learning_iteration) {
		for (size_t number_of_dataset = 0; number_of_dataset < data.height; ++number_of_dataset) {//перебираем все наборы огней светофора (их 6)
			double error_for_all_lights = 0;
			//перемножаем веса на вошедшие данные(перемножение векторов)
			/*pred_vec[0] = weights.matrix[0][0] * data.matrix[i][0];
			pred_vec[1] = weights.matrix[0][1] * data.matrix[i][1];
			pred_vec[2] = weights.matrix[0][2] * data.matrix[i][2];*/
			for (size_t position = 0; position < pred_vec.size(); ++position)
				pred_vec[position] = weights[position] * data.matrix[number_of_dataset][position];
			
				

			//сумма всех весов
			double prediction_accumulate = std::accumulate(pred_vec.begin(), pred_vec.end(), 0.);
			//квадратичная ошибка
			double error = std::pow(prediction_accumulate - goal.matrix[number_of_dataset][0], 2);
			error_for_all_lights += error;
			//разница между предсказанием и верным результатом
			//Положительное значение delta указывает, что прогноз имеет слишком большое
			//значение, а от­рицательное — что слишком маленькое.
			double delta = prediction_accumulate - goal.matrix[number_of_dataset][0];

			/*weights.matrix[0][0] -= alpha * delta * data.matrix[number_of_dataset][0];
			weights.matrix[0][1] -= alpha * delta * data.matrix[number_of_dataset][1];
			weights.matrix[0][2] -= alpha * delta * data.matrix[number_of_dataset][2];*/
			//корректировка весов. получаем приращение weight_delta
			for(size_t position_in_dataset = 0; position_in_dataset < weights.size(); ++position_in_dataset)
				weights[position_in_dataset] -= alpha * delta * data.matrix[number_of_dataset][position_in_dataset];

			std::cout << "error: " << error_for_all_lights << '\t' << " delta: " << delta << std::endl;
			if (error_for_all_lights == 0)return;

			//std::cout << "pred[0] " << pred[0] << "\tpred[1] " << pred[1] << "\tpred[2] " << pred[2] << std::endl;
		}
		std::cout << '\n';
	}
}

//goal - матрица верных значений(идти/не идти), data - матрица огней светофора 
void streerlights_neunet_test(std::vector<double>& weights, const My_matrix<double>& goal, const My_matrix<double>& data) {
	double alpha = 0.001;

	std::vector<double> pred_vec = { 0, 0, 0 };//прогноз
	for (size_t learning_iteration = 0; learning_iteration < 10000; ++learning_iteration) {
		for (size_t number_of_dataset = 0; number_of_dataset < data.height; ++number_of_dataset) {//перебираем все наборы огней светофора (их 6)
			double error_for_all_lights = 0;
			//перемножаем веса на вошедшие данные(перемножение векторов)
			/*pred_vec[0] = weights.matrix[0][0] * data.matrix[i][0];
			pred_vec[1] = weights.matrix[0][1] * data.matrix[i][1];
			pred_vec[2] = weights.matrix[0][2] * data.matrix[i][2];*/
			for (size_t position = 0; position < pred_vec.size(); ++position)
				pred_vec[position] = weights[position] * data.matrix[number_of_dataset][position];



			//сумма всех весов
			double prediction_accumulate = std::accumulate(pred_vec.begin(), pred_vec.end(), 0.);
			//квадратичная ошибка
			double error = std::pow(prediction_accumulate - goal.matrix[number_of_dataset][0], 2);
			error_for_all_lights += error;
			//разница между предсказанием и верным результатом
			//Положительное значение delta указывает, что прогноз имеет слишком большое
			//значение, а от­рицательное — что слишком маленькое.
			double delta = prediction_accumulate - goal.matrix[number_of_dataset][0];
			
			//дельты
			std::vector<double> weight_deltas{ 0, 0, 0 };
			for (int k = 0; k < 3; ++k) {
				weight_deltas[k] = delta * data.matrix[number_of_dataset][k];
			}

			/*weights.matrix[0][0] -= alpha * delta * data.matrix[number_of_dataset][0];
			weights.matrix[0][1] -= alpha * delta * data.matrix[number_of_dataset][1];
			weights.matrix[0][2] -= alpha * delta * data.matrix[number_of_dataset][2];*/
			//корректировка весов. получаем приращение weight_delta
			for (size_t position_in_dataset = 0; position_in_dataset < weights.size(); ++position_in_dataset)
				weights[position_in_dataset] -= alpha * weight_deltas[position_in_dataset];

			

			std::cout << "error: " << error_for_all_lights << '\t' << " delta: " << delta << std::endl;
			if (error_for_all_lights == 0)return;

			//std::cout << "pred[0] " << pred[0] << "\tpred[1] " << pred[1] << "\tpred[2] " << pred[2] << std::endl;
		}
		std::cout << '\n';
	}
}


int main()
{
	//матрица, обозначающая светофор с тремя огоньками
	My_matrix<double> streetlights(6, 3);
	streetlights.matrix = {
		{1, 0, 1},
		{0, 1, 1},
		{0, 0, 1},
		{1, 1, 1},
		{0, 1, 1},
		{1, 0, 1}
	};
	streetlights.print();
	//матрица выходных данныйх. 1 - идти на светофор, 0 - стоять
	My_matrix<double> walk_vs_stop(6, 1);
	walk_vs_stop.matrix = {
		{0},
		{1},
		{0},
		{1},
		{1},
		{0}
	};
	walk_vs_stop.print();
	//матрица весеов для огней светофора
	//My_matrix<double> weights(1, 3);
	//weights.matrix = { {0.5, 0.48, -0.7} };
	//weights.matrix = { {1., 1., 1.} };
	std::vector<double> weights = { 0.48, -0.1, 0.06 };
	streerlights_neunet_test(weights, walk_vs_stop, streetlights);
	std::cout << "final weights:\n";
	for (size_t i = 0; i < weights.size(); ++i) {
		std::cout << weights[i] << '\t';
	}
	std::cout << '\n';
	//double weight = 0.5;
	//double goal_pred = 20;
	//double input = 4;
	////сначала число строк, затем - столбцов
	//size_t height = 4;
	//size_t width = 3;
	//My_matrix<double> mt1(height, width);
	//My_matrix<double> mt2(height, width);
	//mt1.go_over_elements([](double& value) {value = 2; });
	//mt2.go_over_elements([](double& value) {value = 4; });
	//auto tmp = mt1 * mt2;
	//mt1.print();
	//mt2.print();
	//tmp.print();
	//tmp = tmp + mt1;
	//tmp.go_over_elements([](double& value) {value += value; });
	//tmp.print();
	return 0;
}
