#include "BruteForce.hpp"

#include <bitset>
#include <chrono>

void BruteForce::find_one_solution() const {
    int ind(0);
    for (const auto& i : problems_) {
        for (const auto& problem : i) {
            auto& vector(std::get<0>(problem));
            const auto& weight(std::get<1>(problem));
            const auto maxVal(std::pow(2, N));
            bool is_solve(false);
            auto startTime(std::chrono::system_clock::now());
            for (int i(0); i < maxVal; ++i) {
                std::bitset<N> bitset(i);
                weight_t total_weight(0);
                for (int j(0); j < N; ++j)
                    total_weight += bitset[j] == 0 ? 0 : vector[j];
                if (total_weight == weight) {
                    is_solve = true;
                    break;
                };
            }
            if (!is_solve) {
                std::cout << "Unexpected result: the solve wasn't found!\n";
                return;
            }
            auto endTime(std::chrono::system_clock::now());
            double duration(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    endTime - startTime
                )
                    .count()
            );
            std::cout << duration / 1000 << std::endl;
        }
    }
}

void BruteForce::find_all_solutions() const {
    int ind(0);
    std::vector<int> solves_count;
    for (const auto& i : problems_) {
        for (const auto& problem : i) {
            auto& vector(std::get<0>(problem));
            const auto& weight(std::get<1>(problem));
            const auto maxVal(std::pow(2, N));
            bool is_solve(false);
            auto count(0);

            auto startTime(std::chrono::system_clock::now());
            for (int i(0); i < maxVal; ++i) {
                std::bitset<N> bitset(i);
                weight_t total_weight(0);
                for (int j(0); j < N; ++j)
                    total_weight += bitset[j] == 0 ? 0 : vector[j];
                if (total_weight == weight) {
                    is_solve = true;
                    ++count;
                };
            }
            if (!is_solve) {
                std::cout << "Unexpected result: the solve wasn't found!\n";
                return;
            }
            auto endTime(std::chrono::system_clock::now());
            double duration(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    endTime - startTime
                )
                    .count()
            );
            solves_count.push_back(count);
            std::cout << duration / 1000 << std::endl;
        }
    }
    LINE;
    for (const auto& count : solves_count) std::cout << count << std::endl;
}