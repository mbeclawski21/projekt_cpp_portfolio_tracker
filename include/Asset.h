#ifndef ASSET_H
#define ASSET_H

#include <string>
#include <iostream>

class Asset {
protected:
    std::string name;
    std::string symbol;
    double currentPrice;

public:
    Asset(std::string n, std::string s, double p) 
        : name(n), symbol(s), currentPrice(p) {}

    virtual ~Asset() {}

    virtual void displayInfo() const {
        std::cout << "[" << symbol << "] " << name << ": $" << currentPrice << std::endl;
    }

    // Tu bedzie wklejone API do aktualizacji ceny
    virtual void updatePrice(double newPrice) {
        currentPrice = newPrice;
    }

    std::string getSymbol() const { return symbol; }
    double getPrice() const { return currentPrice; }
};

#endif