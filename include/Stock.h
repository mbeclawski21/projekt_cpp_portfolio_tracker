#include "Asset.h"

class Stock : public Asset {
    std::string stockExchange; // np. GPW lub NASDAQ
public:
    Stock(std::string n, std::string s, double p, std::string ex) 
        : Asset(n, s, p), stockExchange(ex) {}

    void displayInfo() const override {
        std::cout << "AKCJA: ";
        Asset::displayInfo();
    }
};