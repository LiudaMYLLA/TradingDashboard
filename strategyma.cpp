#include "strategyma.h"
#include <iostream>
#include <cmath>

// Moving Average Startegy
strategyMA::strategyMA() {}

void strategyMA::getLastSignal(){
    if(this->MAPeriodSlowLast != 0 && this->MAPriodFastLast != 0){
        if(this->MAPeriodFastHistory.size() >= 2 && this->MAPeriodSlowHistory.size() >= 2){
            lastSygnal = NONE;

            int prevLastF = this->MAPeriodFastHistory.size() - 2;
            std::cout << "prevLastF: " << prevLastF << "\n";

            int prevLastS = this->MAPeriodSlowHistory.size() - 2;
            std::cout << "prevLastS: " << prevLastS << "\n";

            float eps = 0.0001 * this->currentCandle.close;
            std::cout << "eps: " << eps << "\n";

            if(this->MAPeriodFastHistory[prevLastF].second <= this->MAPeriodSlowHistory[prevLastS].second
                && this->MAPriodFastLast > this->MAPeriodSlowLast){

                this->lastSygnal = BUY;
                std::cout << "lastSygnal" << this->lastSygnal << "\n";

            }else if(this->MAPeriodFastHistory[prevLastF].second >= this->MAPeriodSlowHistory[prevLastS].second
                       && this->MAPriodFastLast < this->MAPeriodSlowLast){

                this->lastSygnal = SELL;
                std::cout << "lastSygnal" << this->lastSygnal << "\n";

            }else if(fabs(this->MAPriodFastLast - this->MAPeriodSlowLast) < eps){

                this->lastSygnal = NONE;
                std::cout << "lastSygnal" << this->lastSygnal << "\n";

            }else{
                std::cout << "NO BUY, NO SELL, NO NONE!" << "\n";
            }
        }else{
            this->lastSygnal = NONE;
            std::cout << "The history size to small to have calculations" << "\n";
        }
    }else{
        this->lastSygnal = NONE;
    }
}

void strategyMA::calculate(Candle candle){
    this->currentCandle = candle;
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
    if (candles.empty()) {
        std::cout << "strategyMA::init: no candles\n";
        return;
    }

    for (int i = 0; i < candles.size(); ++i){

        const Candle &c = candles[i];
        double closePrice = c.close;

        // Add to slow
        this->periodSlow.push_back(closePrice);
        this->sumPeriodSlow += closePrice;

        if(this->periodSlow.size() > this->pS){
            this->sumPeriodSlow -= this->periodSlow.front();
            this->periodSlow.pop_front();
        }
        // Add to fast
        this->periodFast.push_back(closePrice);
        this->sumPeriodFast += closePrice;

        if(this->periodFast.size() > this->pF){
            this->sumPeriodFast -= this->periodFast.front();
            this->periodFast.pop_front();
        }
        if(this->periodFast.size() == this->pF && this->periodSlow.size() == this->pS){
            calculate(c);
            this->getLastSignal();
        }

    }

    std::cout << "strategyMA::init completed. Fast history: "
    << MAPeriodFastHistory.size() << ", Slow history: "
    << MAPeriodSlowHistory.size() << "\n";
}









