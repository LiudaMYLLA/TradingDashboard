#ifndef VOLATILITYCALC_H
#define VOLATILITYCALC_H

#include "returnCalc.h"

class volatilityCalc : public returnCalc
{
    double volatility = 0.0;
    double sumR();
public:
    volatilityCalc();
    double calculateVolatility();
};

#endif // VOLATILITYCALC_H
