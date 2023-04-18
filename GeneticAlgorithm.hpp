#ifndef GENETIC_ALGORITHM_HPP_INCLUDED_
#define GENETIC_ALGORITHM_HPP_INCLUDED_

#include <algorithm>
#include <bitset>
#include <cmath>
#include <iomanip>

#include "constants.hpp"

enum Breakpoints { NOT_FINISHED = 0, SOLVE_FOUND, STAGNATION, TIME_OUT };

class Chromosome;
// Метод solve возвращает такой кортеж, где есть вся нужная инфа по полученному
// алгоритмом результату: хромосома, принятая за решение, причина остановки,
// значение фитнесс-функции, кол-во сформированных популяций и время работы
using knapsack_solving =
    std::tuple<Chromosome, Breakpoints, weight_t, size_t, double>;

class Chromosome {
   public:
    friend class GeneticAlgorithm;

    Chromosome() : vector_(0), phenotype_(0.0), fitness_(0), viability_(0){};
    Chromosome(const std::bitset<N>& vector) :
        vector_(vector), phenotype_(0.0), fitness_(0), viability_(0){};

    auto operator[](const size_t ind) const { return vector_[ind]; };
    auto operator[](const size_t ind) { return vector_[ind]; };

    auto operator()() const { return vector_; };
    auto& operator()() { return vector_; };

   private:
    std::bitset<N> vector_;
    // Стата хромосомы меняется в текущей популяции классом GeneticAlgorithm
    weight_t phenotype_, fitness_;
    weight_td viability_;
};

class GeneticAlgorithm {
   public:
    GeneticAlgorithm(
        const knapsack& knapsack, const weight_t weight, const size_t size,
        const double chance_generate_thing, const double chance_crossover,
        const double chance_mutation
    ) :
        knapsack_(knapsack),
        weight_(weight),
        size_(size),
        stagnation_counter(0),
        chance_generate_thing_(chance_generate_thing),
        chance_crossover_(chance_crossover),
        chance_mutation_(chance_mutation){};

    knapsack_solving solve();

   private:
    // Перевод двоичного вектора в его фенотип в соотвествии с рюкзачным
    // вектором; Изменяет состояние phenotype_ хромосомы
    Chromosome& phenotype(Chromosome&) const;

    // Вычисление фитнесс-функции для конкретной хромосомы: разница между
    // требуемым (weight_) и фактическим (phenotype_ класса Chromosome)
    // значениями. Обновляет состояние fitness_ хромосомы
    Chromosome& fitness(Chromosome&) const;

    // Вывод информации о популяции (можно отдельно ее перенаправить в файл, а
    // сами результаты выводить в общий поток вывода)
    std::ostream& printStats(std::ostream&) const;

    // Псевдо-случайная генерация начальной популяции: для каждого гена
    // вероятность 0 или 1 равна chance_generate_
    void generateZeroPopulation();

    // Обновляет информацию для всех имеющихся хромосом в популяции (через
    // phenotype(), fitness() а потом обновляя поле viability_ в классе
    // Chromosome, чтобы оператор кроссинговера сам не высчитывал вероятности)
    bool updateStats();

    // Проверка, выполнена ли точка остановка
    Breakpoints isBreakpoint() const;

    // Проверка, больше ли фенотип хромосомы требуемого веса; если да, то он
    // нафиг не нужон
    bool isVectorValid(const Chromosome&) const;

    // Оператор скрещивания: для каждой хромосомы с вероятностью
    // chance_crossover_ меняется с другой хромосомой 1..N-1 генами простым
    // методом рулетки
    std::vector<Chromosome> operator_crossover() const;

    // Оператор мутации. С вероятностью chance_crossover_ инвертирует один из
    // генов применительно к каждой хромосоме из новосозданных потомков
    std::vector<Chromosome>& operator_mutation(std::vector<Chromosome>&) const;

    // Оператор редукции: "отсеивание" самых слабых особей из расширенной
    // популяции до размера size_ популяции
    bool operator_reduction();

    // Параметры задачи о рюкзаке (только те, что нужны: рюкзачный вектор и
    // вес)
    knapsack knapsack_;
    weight_t weight_;
    // Вероятности кроссинговера и мутации, и получить 1 в гене (будем считать,
    // что мы знаем, что доля 1 в векторе колеблется от 0.1 до 0.5)
    double chance_crossover_, chance_mutation_, chance_generate_thing_;
    // Сама популяция и ее размер
    std::vector<Chromosome> population_;
    size_t size_;
    // Статистика текущей популяции: обновляется при вызове updateStats()
    weight_td average_fitness_;
    weight_t optimum_fitness_;
    // Если улучшения среднего значения фитнесс-функции в методе solve() не
    // произошло, счетчик увеличивается; иначе обнуляем
    size_t stagnation_counter;
};

#endif /* GENETIC_ALGORITHM_HPP_INCLUDED_ */