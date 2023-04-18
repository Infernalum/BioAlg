#include "GeneticAlgorithm.hpp"

#include <chrono>

extern std::mt19937_64 eng;
extern std::uniform_int_distribution<weight_t> distr;

Chromosome& GeneticAlgorithm::phenotype(Chromosome& chromo) const {
    size_t i          = 0;
    chromo.phenotype_ = 0;
    for (const auto& thing : knapsack_)
        chromo.phenotype_ += thing * chromo[i++];
    return chromo;
}

Chromosome& GeneticAlgorithm::fitness(Chromosome& chromo) const {
    chromo.fitness_ = std::llabs(weight_ - chromo.phenotype_);
    return chromo;
}

void GeneticAlgorithm::generateZeroPopulation() {
    for (int i = 0; i < size_; ++i) {
        Chromosome chromo;
        do {
            chromo.vector_ = 0x00;
            for (int gene = 0; gene < N; ++gene)
                if ((distr(eng) % 101) <= (chance_generate_thing_ * 100))
                    chromo[gene].flip();
        } while (!isVectorValid(chromo));
        population_.push_back(std::move(chromo));
    }
}

bool GeneticAlgorithm::updateStats() {
    average_fitness_          = 0.0;
    optimum_fitness_          = fitness(phenotype(population_[0])).fitness_;
    weight_td total_viability = 0.0;

    for (auto& chromosome : population_) {
        const auto _phenotype = phenotype(chromosome).phenotype_;
        const auto _fitness   = fitness(chromosome).fitness_;
        average_fitness_ += _fitness;
        if (_fitness < optimum_fitness_) optimum_fitness_ = _fitness;
        total_viability +=
            (chromosome.viability_ = weight_td(weight_) / chromosome.fitness_);
    }
    average_fitness_ /= size_;

    // Метод колеса рулетки: чем меньше значение фитнесс-функции у хромосомы,
    // тем больше будет у нее _viability (вероятность участия в скрещивании)
    for (auto& chromosome : population_)
        chromosome.viability_ *= (100 / total_viability);
    return true;
};

std::vector<Chromosome>& GeneticAlgorithm::operator_mutation(
    std::vector<Chromosome>& vector
) const {
    for (auto& chromosome : vector) {
        if ((distr(eng) % 101) > (chance_mutation_ * 100)) continue;
        // std::cout << "Вектор " << chromosome.vector_ << " поменял ген №";
        Chromosome mutated(chromosome);
        do {
            auto inx = distr(eng) % N;
            mutated.vector_.flip(inx);
        } while (!isVectorValid(phenotype(mutated)));
        chromosome = mutated;
        // std::cout << N - inx << " с " << !chromosome.vector_[inx] << " на "
        //           << chromosome.vector_[inx] << std::endl;
    }
    return vector;
}

std::vector<Chromosome> GeneticAlgorithm::operator_crossover() const {
    std::vector<Chromosome> children;
    for (int i = 0; i < size_; ++i) {
        auto& chromosome = population_[i];
        if ((distr(eng)) % 101 >= (chance_crossover_ * 100)) continue;
        // Определяем второго родителя рулеткой
        int partner;
        int crossover;
        Chromosome child;
        child.vector_ = chromosome();
        do {
            partner       = -1;
            int segment   = distr(eng) % 101;
            double sector = 0.0;
            // Условие на конец нужно потому, что сумма всех viability вещ
            // число и будет равно 99.(9), т.е. если прокнет segment = 100,
            // то условие на > не выполнится все равно
            while (segment > sector && partner != (size_ - 1))
                sector += population_[++partner].viability_;
        } while (partner == i);
        // Одноточечный кроссинговер: crossover находится в промежутке [1,
        // N - 1], т.е. численно показывает, сколько в левой части останется
        // генов от рассматриваемого потомка
        crossover = 1 + distr(eng) % (N - 1);
        for (int j = 0; j < N - crossover; ++j)
            child[j] = population_[partner][j];
        // std::cout << "Родители:\t" << i + 1 << " - " << partner + 1 << "\t ->
        // ";
        //  Вывод в BigEndian
        // for (int j = child.vector_.size() - 1; j >= 0; --j) {
        //     std::cout << child[j];
        //     if (j == crossover) std::cout << '|';
        // }
        // std::cout << '\n';
        if (isVectorValid(phenotype(child)))
            children.push_back(std::move(child));
    }
    return children;
}

bool GeneticAlgorithm::operator_reduction() {
    std::sort(
        population_.begin(), population_.end(),
        [](const Chromosome& left, const Chromosome& right) -> bool {
            return left.viability_ > right.viability_;
        }
    );
    population_ = std::vector(population_.begin(), population_.begin() + size_);
    return true;
};

Breakpoints GeneticAlgorithm::isBreakpoint() const {
    if (stagnation_counter >= POPULATION_LIMIT) return Breakpoints::STAGNATION;
    for (const auto& chromo : population_)
        if (!chromo.fitness_) return Breakpoints::SOLVE_FOUND;
    return Breakpoints::NOT_FINISHED;
};

bool GeneticAlgorithm::isVectorValid(const Chromosome& chromo) const {
    return chromo.phenotype_ > weight_ ? false : true;
}

std::ostream& GeneticAlgorithm::printStats(std::ostream& OS) const {
    OS << "\tИскомый вес: " << weight_ << std::endl;
    LINE;
    for (auto i = 0; i < header.size(); ++i) OS << '|' << header[i].first;
    OS << "|\n";
    auto ind = 0;
    for (const auto& chromosome : population_) {
        OS << std::setfill(' ') << std::left;
        // 1ая колонка
        auto alignment = (header[0].second - std::to_string(ind).size()) / 2;
        OS << '|' << std::string(alignment, ' ')
           << std::setw(header[0].second - alignment) << ++ind;
        // 2ая колонка
        alignment = (header[1].second - chromosome().size()) / 2;
        OS << "|" << std::string(alignment, ' ')
           << std::setw(header[1].second - alignment) << chromosome();
        // 3я колонка
        const auto& phenotype = chromosome.phenotype_;
        alignment = (header[2].second - std::to_string(phenotype).size()) / 2;
        OS << "|" << std::string(alignment, ' ')
           << std::setw(header[2].second - alignment) << phenotype;
        // 4я колонка
        const auto& fitness = chromosome.fitness_;
        alignment = (header[3].second - std::to_string(fitness).size()) / 2;
        OS << "|" << std::string(alignment, ' ')
           << std::setw(header[3].second - alignment) << fitness;
        // 5я колонка
        const auto viability = chromosome.viability_;
        alignment = (header[4].second - std::to_string(viability).size()) / 2;
        OS << "|" << std::string(alignment, ' ')
           << std::setw(header[4].second - alignment) << viability;
        // 6я колонка
        alignment =
            (header[5].second - std::to_string(optimum_fitness_).size()) / 2;
        OS << "|" << std::string(alignment, ' ')
           << std::setw(header[5].second - alignment) << optimum_fitness_;
        // 7я колонка
        alignment =
            (header[6].second - std::to_string(average_fitness_).size()) / 2;
        OS << "|" << std::string(alignment, ' ')
           << std::setw(header[6].second - alignment) << average_fitness_;
        OS << "|\n";
    }
    LINE;
    return OS;
}

knapsack_solving GeneticAlgorithm::solve() {
    auto startTime(std::chrono::system_clock::now());
    // std::cout << "\tГенерация исходной популяции.\n";
    generateZeroPopulation();
    updateStats();
    // printStats(std::cout);
    Breakpoints result;
    size_t population_count = 0;
    while (!(result = isBreakpoint())) {
        auto prev_average_fitness = average_fitness_;
        // std::cout << "\tЭтап селекции: полученные кроссинговером ("
        //           << chance_crossover_ << ") особи:\n";
        auto children             = operator_crossover();
        // std::cout << "\tОператор мутации:\n";
        operator_mutation(children);
        for (auto&& child : children) population_.push_back(child);
        updateStats();
        // std::cout << "\tОбновленная популяция:\n";
        // printStats(std::cout);
        // std::cout << "\tПопуляция после редукции:\n";
        operator_reduction();
        updateStats();
        // printStats(std::cout);
        stagnation_counter = average_fitness_ >= prev_average_fitness
                               ? stagnation_counter + 1
                               : 0;
        ++population_count;
    }
    auto endTime(std::chrono::system_clock::now());
    double duration(std::chrono::duration_cast<std::chrono::milliseconds>(
                        endTime - startTime
    )
                        .count());
    knapsack_solving solving;
    auto solving_vector = std::find_if(
        population_.begin(), population_.end(),
        [&](const Chromosome& el) { return el.fitness_ == optimum_fitness_; }
    );
    std::get<0>(solving) = *solving_vector;
    std::get<1>(solving) = result;
    std::get<2>(solving) = solving_vector->phenotype_;
    std::get<3>(solving) = population_count;
    std::get<4>(solving) = duration / 1000;
    return solving;
}