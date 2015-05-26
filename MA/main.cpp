//
//  main.cpp
//  MA
//
//  Created by 最愛學長的(・ω・)ノ on 2015/5/21.
//  Copyright (c) 2015年 TacoxDD. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include "MA.h"

int main(int argc, const char * argv[]) {

    //std::vector<std::string> dataset{"tai20_5_1.txt", "tai20_10_1.txt", "tai20_20_1.txt", "tai50_5_1.txt", "tai50_10_1.txt", "tai50_20_1.txt", "tai100_5_1.txt", "tai100_10_1.txt", "tai100_20_1.txt" };

    //for (auto filename : dataset) {
        
        for (std::size_t crossover_type = 0; crossover_type < 4; crossover_type += 1) {
            
            std::cout << "Crossever type " << crossover_type << " result:" << std::endl;

            std::vector<int> result;
            for (std::size_t i = 0; i < 100; i += 1) {
                MA ma(100, 0.5, 20, "tai100_20_1.txt");
                ma.initialize();
                ma.evaluateAll();
                
                int generation = 100;
                while (generation--) {
                    ma.tournament();
                    ma.crossover(0);
                    ma.mutation();
                    ma.generationModel();
                    ma.lamarkian();
                }
                //result.push_back(ma.getBest());
                std::cout << ma.getBest() << std::endl;
            }
            
            

            /*
            double sum = std::accumulate(result.begin(), result.end(), 0);
            double mean = sum / result.size();
            double squrtSum = std::inner_product(result.begin(), result.end(), result.begin(), 0.0);
            double var = std::sqrt(squrtSum / result.size() - mean * mean);
            
            std::cout << "Experiment: " << filename << std::endl;
            std::cout << "Max: " << *std::max_element(result.begin(), result.end()) << std::endl;
            std::cout << "Min: " << *std::min_element(result.begin(), result.end()) << std::endl;
            std::cout << "Avg: " << mean << std::endl;
            std::cout << "Var: " << var << std::endl;
            */
            
        }
    //}
    
    
    return 0;
}
