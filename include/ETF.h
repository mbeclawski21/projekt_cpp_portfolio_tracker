#ifndef ETF_H
#define ETF_H

#include "Asset.h"

class ETF : public Asset {
    std::string sector;
public:
    ETF(std::string n, std::string s, double p, std::string sect) 
        : Asset(n, s, p), sector(sect) {}

    std::string getSector() const { return sector; }
};

#endif