#ifndef STRATEGYMA_H
#define STRATEGYMA_H

#include "Candle.h"

#include <deque>
#include <vector>
#include <string>
#include <utility>

enum Signal{
    NONE,
    BUY,
    SELL
};

class strategyMA
{
    std::deque<double> periodFast;
    std::deque<double> periodSlow;

    double sumPeriodFast = 0.0;
    double sumPeriodSlow = 0.0;

    int pF = 20;
    int pS = 50;

    double MAPriodFastLast = 0.0;
    double MAPeriodSlowLast = 0.0;

    std::vector<std::pair<std::string, double>> MAPeriodFastHistory;
    std::vector<std::pair<std::string, double>> MAPeriodSlowHistory;

    Signal lastSygnal;
public:
    strategyMA();

    // updating in every new candle appears
    void update(Candle newCandle);

    void calculate(Candle candle);
    void getLastSignal();

    // getting candles initially
    void init(std::vector<Candle> candles);
};

#endif // STRATEGYMA_H
