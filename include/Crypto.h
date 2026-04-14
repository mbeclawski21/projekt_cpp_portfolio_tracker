#ifndef CRYPTO_H
#define CRYPTO_H
#include "Asset.h"

class Crypto : public Asset {
public:
    Crypto(std::string n, std::string s, double p, double amt) 
        : Asset(n, s, p, amt) {}
};
#endif