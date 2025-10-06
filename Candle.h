#ifndef CANDLE_H
#define CANDLE_H
#include <string>

struct Candle{
    std::string data;
    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
    long volume = 0.0;
};

#endif
