#include <random>

typedef u_long weight_t;

// Общий генератор для всех линкуемых модулей (гениальный костыль бл)
static const size_t SEED = 666;
std::random_device rd;
std::mt19937_64 eng(SEED == 0 ? rd() : SEED);
std::uniform_int_distribution<weight_t> distr;