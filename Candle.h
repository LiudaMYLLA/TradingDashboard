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

    Candle(std::string d, double o, double h, double l, double c, long v):
        data(d), open(o), high(h), low(l), close(c), volume(v){}

    Candle(){}
};

#endif
