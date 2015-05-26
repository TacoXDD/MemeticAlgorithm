#include "MA.h"
#include "RandomRange.h"
#include <vector>
#include <fstream>
#include <iostream>

MA::MA(const int population_size, const double mutation_rate, const int localsearch_times, const std::string &filename)
: population_size_(population_size), mutation_rate_(mutation_rate),localsearch_times_(localsearch_times)
{
    readfile_(filename);
    population_ = Population(population_size_, Chromosome(job_, 0));
    selected_ = std::vector<std::size_t>(population_size_, 0);
    if (job_ == 0 || machine_ == 0)
    {
        std::cout << "[Error] When read file" << std::endl;
    }
    fitness_table_ = std::vector<int>(population_size_, 0);
}

void MA::initialize()
{
    for (std::size_t i = 0; i < population_size_; i += 1)
    {
        int count = 0;
        for (auto &j : population_[i])
        {
            j = count;
            count += 1;
        }
        std::shuffle(population_[i].begin(), population_[i].end(), RandomRange::RandomGenerator);
    }
    
    for (std::size_t i = 0; i < population_size_ / 10; i += 1) {
        population_[i] = localSearch(population_[i]);
    }
}

void MA::evaluateAll()
{
    for (std::size_t i = 0; i < population_size_; i += 1) {
        fitness_table_[i] = fitness_(population_[i]);
    }
    int min = *std::min_element(fitness_table_.begin(), fitness_table_.end());
    if (min < best) {
        best = min;
    }
}

const Chromosome MA::localSearch(Chromosome &chromosome)
{
    Chromosome result(chromosome);
    int loop_times = localsearch_times_;
    int best = fitness_(result);
    bool isFound;
    while (loop_times--)
    {
        isFound = false;
        for (std::size_t i = 0; i < job_ - 1 && !isFound; i += 1)
        {
            for (std::size_t j = i + 1; j < job_ && !isFound; j += 1)
            {
                std::swap(result[i], result[j]);
                int score = fitness_(result);
                if (score < best)
                {
                    best = score;
                    isFound = true;
                }
                else
                {
                    std::swap(result[i], result[j]);
                }
            }
        }
    }
    return result;
}

void MA::lamarkian()
{
    for (std::size_t i = 0; i < (population_size_/10); i += 1) {
        population_[i] = localSearch(population_[i]);
        fitness_table_[i] = fitness_(population_[i]);
    }
    int min = *std::min_element(fitness_table_.begin(), fitness_table_.end());
    if (min < best) {
        best = min;
    }
}

void MA::tournament()
{
    evaluateAll();
    for (std::size_t i = 0; i < population_size_; i += 1) {
        const std::size_t first = RandomRange::random<int>(0, population_size_-1);
        const std::size_t second = RandomRange::random<int>(0, population_size_-1);
        if (fitness_table_[first] < fitness_table_[second]) {
            selected_[i] = first;
        }
        else {
            selected_[i] = second;
        }
    }
    offspring_.clear();
}

void MA::OX(const Chromosome &first_parent, const Chromosome &second_parent)
{
    std::size_t chromosome_size = first_parent.size();
    std::size_t inherit_index = RandomRange::random<int>(0, (int)chromosome_size - 2);
    std::size_t inherit_length = RandomRange::random<int>((int)inherit_index + 1, (int)chromosome_size - 1) - inherit_index;
    
    Chromosome first_temp(second_parent), first_child(first_parent);
    Chromosome second_temp(first_parent), second_child(second_parent);
    
    //mark which had been inherited
    for(std::size_t i = 0; i < chromosome_size; i += 1)
    {
        for(std::size_t j = 0; j < inherit_length; j += 1)
        {
            if(first_temp[i] == first_parent[inherit_index+j])
            {
                first_temp[i] = -1;
            }
            if(second_temp[i] == second_parent[inherit_index+j])
            {
                second_temp[i] = -1;
            }
        }
    }
    
    //add temp to child
    for(std::size_t i = inherit_index + inherit_length, first_addindex = 0, second_addindex = 0; i < chromosome_size + inherit_index; i += 1)
    {
        while(first_temp[first_addindex] == -1 && first_addindex < chromosome_size)
        {
            first_addindex += 1;
        }
        while(second_temp[second_addindex] == -1 && second_addindex < chromosome_size)
        {
            second_addindex += 1;
        }
        
        first_child[i % chromosome_size] = first_temp[first_addindex];
        second_child[i % chromosome_size] = second_temp[second_addindex];
        
        first_addindex += 1;
        second_addindex += 1;
    }
    offspring_.push_back(first_child);
    offspring_.push_back(second_child);
}

void MA::LOX(const Chromosome &first_parent, const Chromosome &second_parent)
{
    std::size_t chromosome_size = first_parent.size();
    std::size_t inherit_index = RandomRange::random<int>(0, (int)chromosome_size - 2);
    std::size_t inherit_length = RandomRange::random<int>((int)inherit_index + 1, (int)chromosome_size - 1) - inherit_index;
    Chromosome first_temp(second_parent), first_child(first_parent);
    Chromosome second_temp(first_parent), second_child(second_parent);
    for (std::size_t i = 0; i < chromosome_size; i += 1)
    {
        for (std::size_t j = 0; j < inherit_length; j += 1)
        {
            if (first_temp[i] == first_parent[inherit_index+j])
            {
                first_temp[i] = -1;
            }
            if (second_temp[i] == second_parent[inherit_index+j])
            {
                second_temp[i] = -1;
            }
        }
    }
    
    for (std::size_t i = 0, k1 = 0, k2 = 0; i < chromosome_size; i += 1)
    {
        if (i == inherit_index)
        {
            i += (inherit_length - 1);
        }
        else
        {
            while (first_temp[k1]  == -1 && k1 < chromosome_size) {
                k1 += 1;
            }
            while (second_temp[k2] == -1 && k2 < chromosome_size) {
                k2 += 1;
            }
            first_child[i]  = first_temp[k1];  if (k1 < chromosome_size) { k1 += 1; }
            second_child[i] = second_temp[k2]; if (k2 < chromosome_size) { k2 += 1; }
        }
    }
    offspring_.push_back(first_child);
    offspring_.push_back(second_child);
}

void MA::PMX(const Chromosome &first_parent, const Chromosome &second_parent)
{
    std::size_t chromosome_size = first_parent.size();
    std::size_t inherit_index  = RandomRange::random<int>(0, (int)chromosome_size - 2);
    std::size_t inherit_length = RandomRange::random<int>((int)inherit_index + 1, (int)chromosome_size - 1) - inherit_index;
    
    Chromosome first_child(first_parent);
    Chromosome second_child(second_parent);
    for (std::size_t i = inherit_index; i < inherit_length + inherit_index; i += 1)
    {
        if (first_parent[i] != second_parent[i])
        {
            for (std::size_t j = 0; j < chromosome_size; j += 1)
            {
                if (first_child[j] == second_parent[i])
                {
                    std::swap(first_child[i], first_child[j]);
                }
                if (second_child[j] == first_parent[i])
                {
                    std::swap(second_child[i], second_child[j]);
                }
            }
        }
    }
    offspring_.push_back(first_child);
    offspring_.push_back(second_child);
}

void MA::CX(const Chromosome &first_parent, const Chromosome &second_parent)
{
    Chromosome first_child(first_parent);
    Chromosome second_child(second_parent);
    std::size_t check_point = RandomRange::random<int>(0, (int)first_parent.size() - 1);
    std::vector<std::size_t> exchange_element;
    const int start_element = first_parent[check_point];
    exchange_element.push_back(check_point);
    while (second_parent[check_point] != start_element)
    {
        for (std::size_t i = 0; i < first_parent.size(); i += 1)
        {
            if (first_parent[i] == second_parent[check_point])
            {
                exchange_element.push_back(i);
                check_point = i;
                break;
            }
        }
    }
    for (std::size_t i = 0; i < exchange_element.size(); i += 1)
    {
        std::swap(first_child[exchange_element[i]], second_child[exchange_element[i]]);
    }
    offspring_.push_back(first_child);
    offspring_.push_back(second_child);
}

void MA::crossover(const int type)
{
    for (std::size_t i = 0; i < selected_.size(); i += 2) {
        switch (type) {
            case 0:
                OX(population_[selected_[i]], population_[selected_[i+1]]);
                break;
            case 1:
                LOX(population_[selected_[i]], population_[selected_[i+1]]);
                break;
            case 2:
                PMX(population_[selected_[i]], population_[selected_[i+1]]);
                break;
            case 3:
                CX(population_[selected_[i]], population_[selected_[i+1]]);
                break;
            default:
                break;
        }
    }
}

void MA::mutation()
{
    for (std::size_t i = 0; i < offspring_.size(); i += 1) {
        if (RandomRange::random<double>(0, 1) < mutation_rate_) {
            const std::size_t first = RandomRange::random<int>(0, (int)offspring_[i].size()-1);
            const std::size_t second = RandomRange::random<int>(0, (int)offspring_[i].size()-1);
            std::swap(offspring_[i][first], offspring_[i][second]);
        }
    }
}

void MA::generationModel()
{
    if (offspring_.size() != population_.size())
    {
        std::cout << "[Error] offspring size != population size" << std::endl;
        std::cout << offspring_.size() << " " << population_.size() << std::endl;
        int i; std::cin >> i;
    }
    for (std::size_t i = 0; i < population_size_; i += 1) {
        population_[i] = offspring_[i];
    }
}

void MA::readfile_(const std::string &filename)
{
    std::string useless;
    std::ifstream fin;
    fin.open(filename.c_str());
    
    fin >> job_ >> machine_ >> useless;
    
    Matrix matrix(machine_, std::vector<int>(job_, 0));
    
    for (std::size_t i = 0; i < machine_; i += 1)
    {
        for (std::size_t j = 0; j < job_; j += 1)
        {
            fin >> matrix[i][j];
        }
    }
    matrix_ = matrix;
}

const int MA::fitness_(const Chromosome& chromosome)
{
    std::vector<int> timespan(matrix_.size() + 1, 0);
    for (std::size_t i = 0; i < matrix_[0].size(); i += 1)
    {
        for (std::size_t j = 1; j <= matrix_.size(); j += 1)
        {
            timespan[j] = std::max(timespan[j - 1], timespan[j]) + matrix_[j - 1][chromosome[i]];
        }
    }
    return timespan[matrix_.size()];
}



