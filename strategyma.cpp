#include "strategyma.h"
#include <iostream>
#include <cmath>

//Moving Average && Crossover Startegy

strategyMA::strategyMA() {}
void strategyMA::update(Candle newCandle){
//NOT TESTED!
    if(this->periodFast.size() >= this->pF){
        this->sumPeriodFast -= this->periodFast.front();
        this->periodFast.pop_front();
    }
    this->periodFast.push_back(newCandle.close);
    this->sumPeriodFast += newCandle.close;

    if(this->periodSlow.size() >= this->pS){
        this->sumPeriodSlow -= this->periodSlow.front();
        this->periodSlow.pop_front();
    }
    this->periodSlow.push_back(newCandle.close);
    this->sumPeriodSlow += newCandle.close;

    if(this->periodFast.size() == this->pF){
        calculateFast(newCandle);
    }
    if(this->periodSlow.size() == this->pS){
        calculateSlow(newCandle);
    }
}

void strategyMA::getLastSignal(){
    if(this->MAPeriodFastHistory.size() >= 2 && this->MAPeriodSlowHistory.size() >= 2){
        int prevLastF = this->MAPeriodFastHistory.size() - 2;
        int prevLastS = this->MAPeriodSlowHistory.size() - 2;
        float eps = 0.0001 * this->currentCandle.close;

        if(this->MAPeriodFastHistory[prevLastF].second <= this->MAPeriodSlowHistory[prevLastS].second
            && this->MAPriodFastLast > this->MAPeriodSlowLast){
            this->lastSygnal = BUY;
            std::cout << "lastSygnal    " << this->lastSygnal << "\n";
        }else if(this->MAPeriodFastHistory[prevLastF].second >= this->MAPeriodSlowHistory[prevLastS].second
            && this->MAPriodFastLast < this->MAPeriodSlowLast){
            this->lastSygnal = SELL;
            std::cout << "lastSygnal    " << this->lastSygnal << "\n";
        }else if(fabs(this->MAPriodFastLast - this->MAPeriodSlowLast) < eps){
            this->lastSygnal = NONE;
            std::cout << "lastSygnal    " << this->lastSygnal << "\n";
        }else{
            this->lastSygnal = NONE;
            std::cout << "lastSygnal    " << this->lastSygnal << "\n";
        }
    }else{
        std::cout << "The history size to small to have calculations" << "\n";
    }
}


void strategyMA::calculateFast(Candle candle){
    this->MAPriodFastLast = this->sumPeriodFast / this->pF;
    std::pair<std::string, double> pairF(candle.data, this->MAPriodFastLast);
    this->MAPeriodFastHistory.push_back(pairF);
}

void strategyMA::calculateSlow(Candle candle){
    this->MAPeriodSlowLast = this->sumPeriodSlow / this->pS;
    std::pair<std::string, double> pairS(candle.data, this->MAPeriodSlowLast);
    this->MAPeriodSlowHistory.push_back(pairS);

    if(this->periodFast.size() == this->pF && this->periodSlow.size() == this->pS){
        this->currentCandle = candle;
        getLastSignal();
    }else{std::cout << "this->periodFast.size() != this->pF or this->periodSlow.size() != this->pS" << "\n";}
}

void strategyMA::init(std::vector<Candle> candles){
    if (candles.empty()) {
        std::cout << "strategyMA::init: no candles\n";
        return;
    }
    for (int i = 0; i < candles.size(); ++i){
        const Candle &c = candles[i];
        double closePrice = c.close;

        // Add to fast
        this->periodFast.push_back(closePrice);
        this->sumPeriodFast += closePrice;

        if(this->periodFast.size() > this->pF){
            this->sumPeriodFast -= this->periodFast.front();
            this->periodFast.pop_front();
        }

        // Add to slow
        this->periodSlow.push_back(closePrice);
        this->sumPeriodSlow += closePrice;

        if(this->periodSlow.size() > this->pS){
            this->sumPeriodSlow -= this->periodSlow.front();
            this->periodSlow.pop_front();
        }

        if(this->periodFast.size() == this->pF){
            calculateFast(c);
        }
        if(this->periodSlow.size() == this->pS){
            calculateSlow(c);
        }
    }
}









