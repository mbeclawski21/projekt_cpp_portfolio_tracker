#ifndef CRYPTO_H
#define CRYPTO_H

#include "Asset.h"

class Crypto : public Asset {
    std::string blockchainNetwork; // np. Bitcoin, Ethereum
public:
    Crypto(std::string n, std::string s, double p, std::string network) 
        : Asset(n, s, p), blockchainNetwork(network) {}

    void displayInfo() const override {
        std::cout << "KRYPTO [" << blockchainNetwork << "]: ";
        Asset::displayInfo();
    }
};

#endif