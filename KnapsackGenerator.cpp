#include "KnapsackGenerator.hpp"

extern std::mt19937_64 eng;
extern std::uniform_int_distribution<weight_t> distr;

std::vector<knapsack> KnapsackGenerator::generate() const {
    std::vector<knapsack> result;
    for (int i(0); i < VECTORS_COUNT; ++i) {
        knapsack knapsack;
        for (int j(0); j < n_; ++j) knapsack.push_back(1 + distr(eng) % amax_);
        result.push_back(std::move(knapsack));
    }
    return result;
}

void KnapsackGenerator::show_knapsaks() const {
    LINE;
    auto ind(0);
    for (const auto& knapsack : knapsacks_) {
        std::cout << ++ind << std::endl;
        for (const auto& chiselko : knapsack) std::cout << chiselko << ' ';
        std::cout << "\nA_max: "
                  << *std::max_element(knapsack.begin(), knapsack.end())
                  << std::endl;
    }
    LINE;
};