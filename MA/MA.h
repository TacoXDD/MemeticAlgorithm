#ifndef __MA__MA__
#define __MA__MA__

#include <vector>

typedef std::vector<int> Chromosome;
typedef std::vector<Chromosome> Population;
typedef std::vector< std::vector<int> > Matrix;

class MA {
public:
    MA(const int, const double, const int, const std::string &);
    void initialize();
    void tournament();
    void mutation();
    void evaluateAll();
    void crossover(const int);
    
    void OX(const Chromosome &, const Chromosome &);
    void LOX(const Chromosome &, const Chromosome &);
    void PMX(const Chromosome &, const Chromosome &);
    void CX(const Chromosome &, const Chromosome &);
    
    const Chromosome localSearch(Chromosome &);
    
    void generationModel();
    void lamarkian();
    
    void readfile_(const std::string &);
    const int fitness_(const Chromosome &);
    
    const int getBest() {return best;}
    
private:
    int best = INT_MAX;
    std::vector<int> fitness_table_;
    Matrix matrix_;
    Population population_, offspring_;
    const int population_size_;
    const double mutation_rate_;
    int localsearch_times_;
    int job_;
    int machine_;
    Chromosome first_ch_;
    std::vector<std::size_t> selected_;
};


#endif /* defined(__MA__MA__) */
