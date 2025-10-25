#ifndef METRICSLOGGER_H
#define METRICSLOGGER_H

#include <string>
#include <fstream>
#include <vector>

class metricsLogger
{
    std::ofstream file;
public:
    metricsLogger(const std::string& filename = "/home/liudmyla-rybak/TradingDashboard/metrics.txt");
    void logVolatility(const double& data);
    void logAverageReturn(const std::vector<double>& data);
};

#endif // METRICSLOGGER_H


// 2
// fstream: file stream
// sub-types>:
// 1) ofstream (out: writing only);
// 2) ifstream (in: reading only);
// 3) stringstream ss(line);

// 3
// methods:
// 1) std::getline(file, line);
// std::stod(string s): converts string to double
// std::stol(string s): converts string to long
