#ifndef KNAPSACK_GENERATOR_HPP_INCLUDED_
#define KNAPSACK_GENERATOR_HPP_INCLUDED_

#include "constants.hpp"

class KnapsackGenerator {
   public:
    KnapsackGenerator(const weight_t amax = AMAX, const int n = N) :
        amax_(amax), n_(n) {
        knapsacks_ = std::move(generate());
    };

    auto get_knapsacks() const { return knapsacks_; };
    void show_knapsaks() const;

   private:
    int n_;
    weight_t amax_;
    std::vector<knapsack> knapsacks_;

    std::vector<knapsack> generate() const;
};

#endif /* KNAPSACK_GENERATOR_HPP_INCLUDED_ */
