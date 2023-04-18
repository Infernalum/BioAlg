#ifndef CONSTANTS_HPP_INCLUDED_
#define CONSTANTS_HPP_INCLUDED_

#include <bits/stdc++.h>

#include <array>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#define LINE                                                                  \
    std::cout                                                                 \
        << "----------------------------------------------------------------" \
           "----------------------------------------------------------------" \
           "----------------------------------------------\n";

typedef u_long weight_t;
// Чтобы вес в виде вещ числа точно влез
typedef long double weight_td;
using knapsack         = std::vector<weight_t>;
// [0] - сам вектор; [1] - целевой вес; [2] - кол-во предметов в решении
using knapsack_problem = std::tuple<knapsack, weight_t, int>;

static const weight_t AMAX(pow(2, 24.0 / 0.8));
static const int VECTORS_COUNT(50);
static const int N(24);
static const int PROBLEMS_PER_VECTOR(10);
static const int POPULATION_LIMIT(3);
static const double min_ratio(0.1);
static const double max_ratio(0.5);

using header_t = std::vector<std::pair<std::string, size_t>>;

static const header_t header{
    std::make_pair(" № Гена ", 8),
    std::make_pair("       Ген (в крокодилах)       ", 32),
    std::make_pair("   Фенотип   ", 13),
    std::make_pair(" Значение фитнесс-функции ", 26),
    std::make_pair(" Приспособленность (в %) ", 25),
    std::make_pair(" Наилучший фитнесс популяции ", 29),
    std::make_pair(" Средний фитнесс ", 17)};

#endif /* CONSTANTS_HPP_INCLUDED_ */
