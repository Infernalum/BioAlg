#ifndef KNAPSACK_PROBLEM_GENERATOR
#define KNAPSACK_PROBLEM_GENERATOR

#include "constants.hpp"

class KnapsackProblemGenerator {
   public:
    KnapsackProblemGenerator(const std::vector<knapsack>& knapsacks) :
        size_(knapsacks[0].size()) {
        generate(knapsacks);
    };

    auto get_problems() const { return problems_; };
    void show_problems() const;

   private:
    std::vector<std::vector<knapsack_problem>> problems_;
    int size_;

    void generate(const std::vector<knapsack>&);
};

#endif /* KNAPSACK_PROBLEM_GENERATOR */
