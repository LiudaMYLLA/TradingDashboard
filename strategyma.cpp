#include "strategyma.h"
#include <iostream>

strategyMA::strategyMA() {}

void strategyMA::getLastSignal(){
    if(this->MAPriodFastLast > this->MAPeriodSlowLast){
        this->lastSygnal = {BUY};
    }
}

void strategyMA::calculate(Candle candle){
    if(this->periodFast.size() == this->pF && this->periodSlow.size() == this->pS){
        this->MAPriodFastLast = this->sumPeriodFast / this->pF;
        this->MAPeriodSlowLast = this->sumPeriodSlow / this->pS;

        // creating MA history
        std::pair<std::string, double> pairF(candle.data, this->MAPriodFastLast);
        std::pair<std::string, double> pairS(candle.data, this->MAPeriodSlowLast);
        this->MAPeriodFastHistory.push_back(pairF);
        this->MAPeriodSlowHistory.push_back(pairS);

    }else{
        std::cout << "Method calculate was called while periodFast: " <<
            this->periodFast.size() << " not equal to pF: " <<
            this->pF << "\n";

        std::cout << "Method calculate was called while periodSlow: " <<
            this->periodSlow.size() << " not equal to pS: " <<
            this->pS << "\n";
    }
}

void strategyMA::update(Candle newCandle){

    if(this->periodFast.size() >= this->pF){
        this->sumPeriodFast -= this->periodFast.front();
        this->periodFast.pop_front();
    }

    if(this->periodSlow.size() >= this->pS){
        this->sumPeriodSlow -= this->periodSlow.front();
        this->periodSlow.pop_front();
    }

    this->periodFast.push_back(newCandle.close);
    this->periodSlow.push_back(newCandle.close);

    this->sumPeriodFast += newCandle.close;
    this->sumPeriodSlow += newCandle.close;

    if(this->periodFast.size() == this->pF && this->periodSlow.size() == this->pS){
        calculate(newCandle);
    }
}

void strategyMA::init(std::vector<Candle> candles){
    Candle lastCandle = candles.back();

    int startS;

    if(candles.size() < this->pS){
        startS = 0;
    }else{
        startS = candles.size() - this->pS;
    }

    for (int i = startS; i < candles.size(); ++i){
        double closePrice = candles[i].close;
        this->periodSlow.push_back(closePrice);
        this->sumPeriodSlow += closePrice;
    }

    int startF;

    if(candles.size() < this->pF){
        startF = 0;
    }else{
        startF = candles.size() - this->pF;
    }

    for (int j = startF; j < candles.size(); ++j){
        double closePrice = candles[j].close;
        this->periodFast.push_back(closePrice);
        this->sumPeriodFast += closePrice;
    }

    if(this->periodFast.size() == this->pF && this->periodSlow.size() == this->pS){
        calculate(lastCandle);
    }
}









