#include <vector>
#include <any>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <cmath>

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

int main()
{
	double weight = 0.5;
	double goal_pred = 20;
	double input = 4;
	neural_network(weight, goal_pred, input);
	

	return 0;
}
