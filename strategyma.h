#ifndef STRATEGYMA_H
#define STRATEGYMA_H

#include "Candle.h"

#include <deque>
#include <vector>

class strategyMA
{
    std::deque<double> periodFast;
    std::deque<double> periodSlow;
public:
    strategyMA();

    // updating in every new candle appears
    void update();

    void calculate();
    void getLastSignal();

    // getting candles initially
    void init(std::vector<Candle> candles);
};

#endif // STRATEGYMA_H
