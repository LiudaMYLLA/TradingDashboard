#include "returnCalc.h"
#include <iostream>
#include <cmath>

returnCalc ::returnCalc () {}

void returnCalc::avarageReturnCalculation(){
    std::cout <<"Now we are calculating in avarageReturnCalculation func! \n";
    std::cout << "this->returns.size(): " <<this->returns.size() << "\n";
    for(int i = 0; i < this->returns.size(); ++i){
        double r = this->returns[i];
        this->sum += r;
    }
    this->avarageR = this->sum/this->returns.size();
    std::cout << "this->avarageR: "<< this->avarageR << "\n";
}

std::vector<double> returnCalc::returnCalculation(std::vector<Candle>& candles){
    if(candles.size() < 1) std::cout << "candles not enough for calculations" << "\n";

    std::cout <<"Candles.size()"<< candles.size() << "\n";
    // start calculating from second one candles in massive
    for(int i = 1;i < candles.size(); ++i){
        double closeCurrent = candles[i].close;
        double closeLast = candles[i-1].close;

        this->R = (closeCurrent - closeLast) / closeLast;
        std::cout << "Return: " << this->R << "  ";
        if(this->R > 0) {std::cout << "Return reflected increase" << "\n";}
        else{std::cout << "Return reflected decrease" << "\n";}
        double rounded = std::round(this->R * 10000.0) / 10000.0;
        this->returns.push_back(rounded);
        std::cout <<"This return was succesfully pushed to this->returns massive! \n";
    }
    avarageReturnCalculation();
    return this->returns;
}

