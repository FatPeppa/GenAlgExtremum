#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#define _USE_MATH_DEFINES

using namespace std;

const int max_pop = 200;

long double Func(double x, double y) {
	return (3 * x * x + x * y + 2 * y * y - x - 4 * y); //p1
	//return pow(y, 2) + 2 * x * y - 4 * x - 2 * y - 3; //p2
	//return sin(sqrt(y / pow(x, 2))); //p13
	//return pow(x-2, 2)+3*pow(y, 2); //p37
	//return 1 - sqrt(pow(x, 3) + pow(y, 2)); //p29
}

void sort(long double fitness[max_pop], double population[2][max_pop]) { //сортировка поколений по убыванию fitness
	for (int i = 0; i < max_pop; i++) {
		for (int j = max_pop - 1; j > i; j--) {
			if (fitness[j] < fitness[j - 1]) {
				swap(fitness[j], fitness[j - 1]);
				swap(population[0][j], population[0][j - 1]);
				swap(population[1][j], population[1][j - 1]);
			}
		}
	}
}

void print_results(long double fitness[max_pop], double population[2][max_pop]) {
	cout << "X: ";

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < max_pop; j++) 
			cout << population[i][j] << "	";
		if (i == 0) 
			cout << endl << "Y: ";
	}

	cout << endl << "Function: ";

	for (int i = 0; i < max_pop; i++) 
		cout << fitness[i] << "	";
	cout << endl << endl;
}

double mutation(double num1, double num2) {
	const int corNum = 1000000000;
	return fabs((double)((rand() * corNum) % (int)((num2 - num1) * corNum) + 1) / corNum) + num1;
}

double inversion(double x, double P) {
	static int s = 0;
	s %= 2;

	s++;
	if (s == 0) return x += P;
	else return x -= P;
}

void GenAlg_min(double population[2][max_pop], double x1, double x2, double P) {
	int k = max_pop - 1;

	for (int i = 0; i < 20; i++) {																		//!!!!
		population[0][k], population[1][k] = inversion(population[0][i], P), inversion(population[1][i], P); k--;
		population[0][k], population[1][k] = inversion(population[0][i], P), inversion(population[1][i], P); k--;
	}
	for (int i = 0; i < 10; i++) {
		for (int j = i + 1; j < 10; j++) {
			population[0][k] = (population[0][i] + population[0][j]) / 2;
			population[1][k] = (population[1][i] + population[1][j]) / 2;
			k--;
		}
	}
	while (k > 0) {
		population[0][k] = mutation(x1, x2);
		population[1][k] = mutation(x1, x2);
		k--;
	}
}

void GenAlg_max(double population[2][max_pop], double x1, double x2, double P) {
	int k = 0;

	for (int i = 0; i < 20; i++) {																		//!!!!
		population[0][k], population[1][k] = inversion(population[0][i], P), inversion(population[1][i], P); k++;
		population[0][k], population[1][k] = inversion(population[0][i], P), inversion(population[1][i], P); k++;
	}
	for (int i = 0; i < 10; i++) { //скрещивание
		for (int j = i + 1; j < 10; j++) {
			population[0][k] = (population[0][i] + population[0][j]) / 2;
			population[1][k] = (population[1][i] + population[1][j]) / 2;
			k++;
		}
	}

	while (k < max_pop - 1) { //мутация
		population[0][k] = mutation(x1, x2);
		population[1][k] = mutation(x1, x2);
		k++;
	}
}

int main()
{
	double population_min[2][max_pop];
	double population_max[2][max_pop];
	double predmin, predmax;
	long double fitness[max_pop];
	int generation = 0;

	srand(time(NULL));

	for (int i = 0; i < 2; i++) 
		for (int j = 0; j < max_pop; j++)
			population_min[i][j] = rand() % 21 - 10 + 1; //генерация первого поколения
	for (int i = 0; i < max_pop; i++)
		fitness[i] = Func(population_min[0][i], population_min[1][i]);
	sort(fitness, population_min);

	for (int i = 0; i < 2; i++) 
		for (int j = 0; j < max_pop; j++)
			population_max[i][j] = population_min[i][j]; //генерация первого поколения

	do {
		predmin = fitness[0];
		generation++;
		GenAlg_min(population_min, -20.0, 20.0, 0.000001);

		for (int i = 0; i < max_pop; i++)
			fitness[i] = Func(population_min[0][i], population_min[1][i]);
		sort(fitness, population_min);
	} while (fabs(predmin - fitness[0]) > 0.00000000000001 && generation < 2000 || generation < 50);

	if (abs(fitness[0]) < 100)
		cout << "Result: " << endl << "X: " << population_min[0][0] << endl << "Y: " <<
		population_min[1][0] << endl << "Function min: " << fitness[0] << endl << "generations: " << generation;
	else 
		cout << "Min limit does not exist or it is too far";

	cout << endl << endl;
	generation = 0;
	
	do {
		predmax = fitness[max_pop - 1];
		generation++;
		GenAlg_max(population_max, -10.0, 10.0, 0.000001);

		for (int i = 0; i < max_pop; i++)
			fitness[i] = Func(population_max[0][i], population_max[1][i]);
		sort(fitness, population_max);
	} while (fabs(predmax - fitness[max_pop - 1]) > 0.00000000000001 && generation < 2000 || generation < 50);

	if (abs(fitness[max_pop - 1]) < 100)
		cout << "Result: " << endl << "X: " << population_max[0][max_pop - 1] << endl << "Y: " <<
		population_max[1][max_pop - 1] << endl << "Function max: " << fitness[max_pop - 1] << endl << "generations: " << generation;
	else 
		cout << "Max limit does not exist or it is too far";
}

