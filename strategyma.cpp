#include "strategyma.h"

strategyMA::strategyMA() {}

void strategyMA::calculate(){

}

void strategyMA::update(){

}

void strategyMA::getLastSignal(){

}

void strategyMA::init(std::vector<Candle> candles){
    int NS = 50;
    int startS;

    if(candles.size() < NS){
        startS = 0;
    }else{
        startS = candles.size() - NS;
    }

    for (int i = startS; i < candles.size(); ++i){
        double closePrice = candles[i].close;
        this->periodSlow.push_back(closePrice);
    }

    int NF = 20;
    int startF;

    if(candles.size() < NF){
        startF = 0;
    }else{
        startF = candles.size() - NF;
    }

    for (int j = startF; j < candles.size(); ++j){
        double closePrice = candles[j].close;
        this->periodFast.push_back(closePrice);
    }
}









