#include "BruteForce.hpp"
#include "GeneticAlgorithm.hpp"
#include "KnapsackGenerator.hpp"
#include "KnapsackProblemGenerator.hpp"

void show_solve(
    const knapsack_problem& problem, const knapsack_solving& solved, size_t& inx
) {
    //++inx;
    if (std::get<1>(solved) == Breakpoints::STAGNATION) return;
    std::cout << "\tЗадача №" << ++inx << std::endl;
    LINE;
    std::cout << "Искомый вес: " << std::get<1>(problem) << std::endl;
    std::cout << "Результаты:\n\tПолученный рюкзачный вектор:\t\t"
              << std::get<0>(solved)()
              << "\n\tПричина завершения алгоритма:\t\t";
    switch (std::get<1>(solved)) {
        case Breakpoints::SOLVE_FOUND:
            std::cout << "Найдено точное решение.\n";
            break;
        case Breakpoints::STAGNATION:
            std::cout << "Попадание в локальный минимум.\n";
            break;
        case Breakpoints::TIME_OUT:
            std::cout << "Превышение времени работы алгоритма более чем в 2 "
                         "раза по сравнению с полным перебором.\n";
            break;
        default:
            std::cout << "Unknown error!\n";
            break;
    }
    std::cout << "\tЗначение фитнесс-функции:\t\t" << std::get<2>(solved)
              << "\n\tТочность (в %):\t\t\t\t"
              << (weight_td)std::get<2>(solved) / std::get<1>(problem) * 100
              << "\n\tКол-во сгенерированных популяций:\t"
              << std::get<3>(solved) << "\n\tВремя работы:\t\t\t\t "
              << std::get<4>(solved) << std::endl;
    LINE;
}

int main(int argc, char* argv[]) {
    KnapsackGenerator knGen;
    // Table 1
    // knGen.show_knapsaks();
    auto vectors = knGen.get_knapsacks();
    KnapsackProblemGenerator problems(vectors);
    auto problem_vector = problems.get_problems();
    // Table 2
    // problems.show_problems();
    // Table 3
    // BruteForce brute(problems.get_problems());
    // brute.find_all_solutions();
    // Table 4
    size_t inx          = 1;
    for (const auto& analogical_problems : problem_vector) {
        for (const auto& problem : analogical_problems) {
            GeneticAlgorithm GA(
                std::get<0>(problem), std::get<1>(problem), 1500, 0.3, 0.9, 0.2
            );
            auto result = GA.solve();
            // show_solve(problem, result, inx);
            // std::cout << std::get<4>(result) << std::endl;
            std::cout << (weight_td)std::get<2>(result) / std::get<1>(problem) *
                             100
                      << std::endl;
        }
    }
    return 0;
}
