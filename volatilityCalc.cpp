#include "volatilityCalc.h"
#include <cmath>
#include <iostream>

volatilityCalc::volatilityCalc() {}

double volatilityCalc::sumR(){
    std::cout << "We are in function sumR! \n";
    std::vector<double> sums;
    double sum = 0.0;

    if(this->returns.size() == 0 || this->avarageR == 0)std::cerr << "Not enough data" << "\n";

    std::cout <<"this->returns.size(): " << this->returns.size() << "\n";
    for(int i = 0; i < this->returns.size(); ++i){
      double pow = std::pow(this->returns[i] - this->avarageR, 2);
      std::cout<< "pow: " << pow << "\n";
      sums.push_back(pow);
    }
    std::cout<< "Sums.size(): " << sums.size() << "\n";
    for(const double& s: sums){
        sum += s;
    }
    std::cout <<"Sum: " << sum << "\n";
    return sum;
}

double volatilityCalc::calculateVolatility(){
    std::cout << "We are in function calculateVolatility! \n";
    double sum = sumR();
    std::cout << "We are in function calculateVolatility! \n";
    std::cout <<"Sum: " << this->sum << "\n";

    double division;
    if(this->returns.size() > 2){
        division = sum/(this->returns.size() - 1);
        std::cout << "Division: " << division << "\n";
    }else{std::cerr << "Not enough return data to calculate volatility.\n";}

    this->volatility = std::sqrt(division); // about rounding later
    std::cout << "Volatility: " << this->volatility << "\n";

    double rounded = std::round(this->volatility * 10000.0) / 10000.0;
    return rounded;
}
