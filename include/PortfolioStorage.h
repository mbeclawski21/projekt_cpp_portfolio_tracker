#ifndef PORTFOLIO_STORAGE_H
#define PORTFOLIO_STORAGE_H

#include "Portfolio.h"

namespace PortfolioStorage {
    void SavePortfolio(const Portfolio& p);
    void LoadPortfolio(Portfolio& p);
}

#endif