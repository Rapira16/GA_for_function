#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

// Функция, для которой мы ищем экстремум
double function(double x, double y) {
    return (1.3*1.3*1.3/(pow((x-2), 2)+1.3*1.3)) + (1.3*1.3*1.3*1.3/(pow((y+1), 2)+1.3*1.3)) +
    (1.3*1.3*1.3*1.3/(pow((x+2), 2)+1.3*1.3)) + (1.3*1.3*1.3*1.3/(pow((y+1), 2)+1.3*1.3));
}

// Класс для представления особей в генетическом алгоритме
class Individual {
public:
    double x;
    double y;
    double fitness;

    Individual() {
        x = 0.0;
        y = 0.0;
        fitness = 0.0;
    }

    Individual(double x_, double y_) {
        x = x_;
        y = y_;
        fitness = 0.0;
    }
};

// Функция для вычисления приспособленности особи
double calculateFitness(Individual& individual) {
    // В данном случае мы просто присваиваем значение функции
    double result = function(individual.x, individual.y);
    return result;
}

// Функция для сортировки особей по убыванию приспособленности
bool compareFitness(const Individual& a, const Individual& b) {
    return a.fitness > b.fitness;
}

// Функция для создания новой популяции особей
std::vector<Individual> createNewPopulation(const std::vector<Individual>& population, int populationSize) {
    std::vector<Individual> newPopulation;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);  // Диапазон для случайной генерации координат особей

    // Отбираем лучшие особи для сохранения в новой популяции (элитарный отбор)
    int elitesCount = populationSize * 0.1;  // 10% лучших особей
    for (int i = 0; i < elitesCount; ++i) {
        newPopulation.push_back(population[i]);
    }

    // Генерируем новые особи с помощью скрещивания
    int offspringCount = populationSize - elitesCount;
    for (int i = 0; i < offspringCount; ++i) {
        int parentIndex1 = i % elitesCount;  // Выбираем первого родителя из лучших особей
        int parentIndex2 = (i + 1) % elitesCount;  // Выбираем второго родителя из лучших особей

        Individual offspring;

        // Скрещиваем координаты родителей с помощью случайной пропорции
        double ratio = dis(gen);  // Случайное число от -10.0 до 10.0
        offspring.x = population[parentIndex1].x * ratio + population[parentIndex2].x * (1 - ratio);
        offspring.y = population[parentIndex1].y * ratio + population[parentIndex2].y * (1 - ratio);

        // Добавляем мутацию с некоторой вероятностью
        if (dis(gen) < 0.1) {  // Вероятность мутации - 10%
            offspring.x += dis(gen) * 0.1;  // Мутируем координату особи
            offspring.y += dis(gen) * 0.1;
        }

        newPopulation.push_back(offspring);
    }

    return newPopulation;
}

// Функция для вывода результатов
void printResult(const Individual& bestIndividual, int iteration) {
    std::cout << "Best individual found after " << iteration << " iterations:" << std::endl;
    std::cout << "x = " << bestIndividual.x << ", y = " << bestIndividual.y << std::endl;
    std::cout << "Fitness = " << bestIndividual.fitness << std::endl;
}

int main() {
    int populationSize = 100;  // Размер начальной популяции
    int numIterations = 1000;  // Количество итераций генетического алгоритма

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0);  // Диапазон для случайной генерации начальной популяции

    std::vector<Individual> population;
    for (int i = 0; i < populationSize; ++i) {
        double x = dis(gen);
        double y = dis(gen);
        population.push_back(Individual(x, y));
    }

    for (int i = 0; i < numIterations; ++i) {
        // Вычисляем приспособленность для каждой особи
        for (auto& individual : population) {
            individual.fitness = calculateFitness(individual);
        }

        // Сортируем популяцию по убыванию приспособленности
        std::sort(population.begin(), population.end(), compareFitness);

        // Выводим текущий лучший результат
        if (i % 100 == 0) {  // Выводим результат каждые 100 итераций
            printResult(population[0], i);
        }

        // Создаем новую популяцию особей
        population = createNewPopulation(population, populationSize);
    }

    // Выводим итоговый лучший результат
    printResult(population[0], numIterations);

    return 0;
}