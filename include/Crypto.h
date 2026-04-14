#ifndef CRYPTO_H
#define CRYPTO_H

#include "Asset.h"

class Crypto : public Asset {
    std::string blockchainNetwork;
public:
    Crypto(std::string n, std::string s, double p, std::string network) 
        : Asset(n, s, p), blockchainNetwork(network) {}

    std::string getNetwork() const { return blockchainNetwork; }
};

#endif