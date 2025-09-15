#ifndef DATALOADER_H
#define DATALOADER_H
#include "Candle.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class DataLoader
{
    std::ifstream file;
public:
    DataLoader(const std::string& filename);

    std::vector<Candle> loadFromCSV();

    ~DataLoader();
};

#endif

// 1
// loadFromCSV is static: l can call it without creating a DataLoader object.

// 2
// fstream: file stream
// sub-types>:
// 1) ofstream (out: writing only);
// 2) ifstream (in: reading only);
// 3) stringstream ss(line);

// 3
// methods:
// 1) std::getline(file, line);
