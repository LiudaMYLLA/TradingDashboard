#ifndef RETURNCALC_H
#define RETURNCALC_H
#include <vector>
#include "candle.h"

class returnCalc
{
    void avarageReturnCalculation();
protected:
    std::vector<double> returns;
    double R = 0.0;
    double sum = 0.0;
    double avarageR = 0.0;
public:
    returnCalc();
    std::vector<double> returnCalculation(std::vector<Candle>& candles);
};

#endif // RETURNCALC_H
