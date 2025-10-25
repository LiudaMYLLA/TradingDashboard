#include "metricslogger.h"

#include <iostream>
metricsLogger::metricsLogger(const std::string& filename) :
    file(filename) {
    if(!file.is_open()){
        throw std::runtime_error("Cannot open file");
    }else {
        std::cout << "File opened successfully: " << filename << "\n";
    }
}

void metricsLogger::logAverageReturn(const std::vector<double>& data){
    this->file << "Average: ";
    for(const double& d: data){
        this->file << d << ", ";
    }
    this->file << "\n";
}

void metricsLogger::logVolatility(const double& data){
    this->file <<"Volatility: "<< data << "\n";
    std::cout << "The data has succesfully written to metrics.txt!" << "\n";
}
