#include "KnapsackProblemGenerator.hpp"

#include <unordered_set>

extern std::mt19937_64 eng;
extern std::uniform_int_distribution<weight_t> distr;

void KnapsackProblemGenerator::generate(const std::vector<knapsack>& knapsacks
) {
    int min = std::ceil(size_ * min_ratio);
    int max = std::round(size_ * max_ratio);
    for (const auto& knapsack : knapsacks) {
        std::vector<knapsack_problem> problems(PROBLEMS_PER_VECTOR);
        // Генерим решения
        for (auto i(0); i < PROBLEMS_PER_VECTOR; ++i) {
            std::get<0>(problems[i]) = knapsack;
            int count                = min + distr(eng) % (max - min + 1);
            std::get<2>(problems[i]) = count;
            // td::cout << count << std::endl;
            std::unordered_set<size_t> indexes;
            while (indexes.size() < count) indexes.insert(distr(eng) % size_);
            weight_t problem_weight(0);
            for (const auto& ind : indexes) {
                // std::cout << ind << ' ';
                problem_weight += knapsack[ind];
            }
            // std::cout << std::endl;
            std::get<1>(problems[i]) = problem_weight;
        }
        problems_.push_back(problems);
    };
};

void KnapsackProblemGenerator::show_problems() const {
    auto size = problems_[0];
    auto ind  = 0;
    for (int i(0); i < VECTORS_COUNT; ++i) {
        // std::cout << std::endl << i + 1 << std::endl;
        auto problems = problems_[i];

        // for (const auto& problem : problems)
        //     std::cout << std::get<1>(problem) << std::endl;
        // std::cout << std::endl;
        for (const auto& problem : problems) {
            // std::cout << (double)std::get<2>(problem) / size_ << std::endl;
            std::cout << std::get<2>(problem) << std::endl;
        }
        // std::cout << std::endl;
    }
}