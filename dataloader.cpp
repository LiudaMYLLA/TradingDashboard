#include "dataloader.h"

DataLoader::DataLoader(const std::string& filename):
    file(filename){
    if(!file.is_open()){
        throw std::runtime_error("Cannot open file");
    }
}

std::vector<Candle> DataLoader::loadFromCSV(){
    std::vector<Candle> vData;
    std::string line;

    std::getline(file, line);
    bool success = static_cast<bool>(std::getline(file, line)); // spend the header, write the data in the line
    while(success){
        Candle candle;
        std::stringstream ss(line);
        std::string token;
        int counter = 0;
        while(std::getline(ss, token, ',')){ // using pattern
            if(counter == 0){
                candle.data = token;
            }else if(counter == 1){
                try{
                    candle.open = std::stod(token);
                }catch(const std::exception& e){
                    std::cerr << "Error parsing number: " << token << "(" << e.what() << ")\n";
                    candle.open = 0;
                };
            }else if(counter == 2){
                try{
                    candle.high = std::stod(token);
                }catch(const std::exception& e){
                    std::cerr << "Error parsing number: " << token << "(" << e.what() << ")\n";
                    candle.high = 0;
                };
            }else if(counter == 3){
                try{
                    candle.low = std::stod(token);
                }catch(const std::exception& e){
                    std::cerr << "Error parsing number: " << token << "(" << e.what() << ")\n";
                    candle.low = 0;
                };
            }else if(counter == 4){
                try{
                    candle.close = std::stod(token);
                }catch(const std::exception& e){
                    std::cerr << "Error parsing number: " << token << "(" << e.what() << ")\n";
                    candle.close = 0;
                };
            }else if(counter == 5){
                try{
                   candle.volume = std::stol(token);
                }catch(const std::exception& e){
                    std::cerr << "Error parsing number: " << token << "(" << e.what() << ")\n";
                    candle.volume = 0;
                };
            }
            ++counter;
        }
        vData.push_back(candle);
        success = static_cast<bool>(std::getline(file, line)); // reading starting from next line
    }
    return vData;
}

DataLoader::~DataLoader(){
    if(file.is_open()){
        file.close();
    }
}

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
