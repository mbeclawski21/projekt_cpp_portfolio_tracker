#ifndef ETF_H
#define ETF_H

#include "Asset.h"

class ETF : public Asset {
    std::string sector; // np. S&P500
public:
    ETF(std::string n, std::string s, double p, std::string sect) 
        : Asset(n, s, p), sector(sect) {}

    void displayInfo() const override {
        std::cout << "ETF [" << sector << "]: ";
        Asset::displayInfo();
    }
};

#endif