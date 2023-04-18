#ifndef BRUTEFORCE_HPP_INCLUDED_
#define BRUTEFORCE_HPP_INCLUDED_

#include "constants.hpp"

class BruteForce {
   public:
    BruteForce(const std::vector<std::vector<knapsack_problem>>& problems) :
        problems_(problems){};

    void find_one_solution() const;

    void find_all_solutions() const;

   private:
    std::vector<std::vector<knapsack_problem>> problems_;
};

#endif /* BRUTEFORCE_HPP_INCLUDED_ */
