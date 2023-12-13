#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

int ichek(std::string x = "") {
    bool b, t = false;
    if (x.length() == 0) {
        t = true;
    }
    while (true) {
        if (t) {
            std::cin >> x;
        }
        b = true;
        if ((x[0] == '-') || (x[0] >= '0' && x[0] <= '9')) {
            for (int i = 1; i < x.length(); ++i) {
                if (not((x[i] >= '0' && x[i] <= '9'))) {
                    b = false;
                    break;
                }
            }
            try {
                if (b) {
                    return stoi(x);
                }
            }
            catch (std::out_of_range const &e) {
                std::cout << "\ntoo big for integer" << std::endl;
            }
        }
        std::cout << "Wrong Value. Try again: " << std::endl;
        std::cin.clear();
        std::cin.ignore();
    }
}

// Функция, для которой мы ищем экстремум
double function(double x, double y) {
    return pow(x, 2) + pow(y, 2);
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
bool compareFitness_1(const Individual& a, const Individual& b) {
    return a.fitness > b.fitness;
}
// Функция для сортировки особей по возрастпнию приспособленности
bool compareFitness_2(const Individual& a, const Individual& b) {
    return a.fitness < b.fitness;
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
    std::cout << "Enter population size: " << std::endl;
    int populationSize = ichek();  // Размер начальной популяции
    std::cout << "Enter amount of iterations (ends with 00)" << std::endl;
    int numIterations = ichek();  // Количество итераций генетического алгоритма
    std::string extreme;
    while (true) {
        std::cout << "min or max of extreame: " << std::endl;
        std::cin >> extreme;
        if (extreme == "min" || extreme == "max") break;
        std::cout << "try again" << std::endl;
    }


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

        // Сортируем популяцию по приспособленности в зависимости от выбора экстремума
        if (extreme == "min"){
            std::sort(population.begin(), population.end(), compareFitness_2);
        } else {
            std::sort(population.begin(), population.end(), compareFitness_1);
        }

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
