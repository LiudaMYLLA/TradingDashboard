#ifndef CANDLE_H
#define CANDLE_H
#include <string>

struct Candle{
    std::string data;
    double open;
    double high;
    double low;
    double close;
    long volume;
};

#endif
