#include <iostream>
#include <string>
#include "Portfolio.h"
#include "Stock.h"
#include "Crypto.h"

void displayMenu() {
    std::cout << "\n=== PortfolioTracker - Menu ===\n";
    std::cout << "1. Dodaj Akcje\n";
    std::cout << "2. Dodaj Kryptowalute\n";
    std::cout << "3. Pokaz zawartosc portfela\n";
    std::cout << "0. Wyjdz z programu\n";
    std::cout << "Wybierz opcje: ";
}

int main() {
    Portfolio myPortfolio;
    int choice = -1;

    std::cout << "Witaj w Tracker Portfolio!\n";

    while (choice != 0) {
        displayMenu();
        std::cin >> choice;

        if (choice == 1) {
            std::string name, symbol, exchange;
            double price;
            
            std::cout << "Podaj nazwe (jedno slowo, np. Apple): ";
            std::cin >> name;
            std::cout << "Podaj symbol (np. AAPL): ";
            std::cin >> symbol;
            std::cout << "Podaj cene zakupu (np. 150.5): ";
            std::cin >> price;
            std::cout << "Podaj gielde (np. NASDAQ): ";
            std::cin >> exchange;

            myPortfolio.addAsset(std::make_unique<Stock>(name, symbol, price, exchange));
            std::cout << "Akcja dodana do portfela!\n";

        } else if (choice == 2) {
            std::string name, symbol, network;
            double price;
            
            std::cout << "Podaj nazwe (np. Bitcoin): ";
            std::cin >> name;
            std::cout << "Podaj symbol (np. BTC): ";
            std::cin >> symbol;
            std::cout << "Podaj cene zakupu: ";
            std::cin >> price;
            std::cout << "Podaj siec (np. BTC): ";
            std::cin >> network;

            myPortfolio.addAsset(std::make_unique<Crypto>(name, symbol, price, network));
            std::cout << "[✓] Krypto dodane do portfela!\n";

        } else if (choice == 3) {
            myPortfolio.displayPortfolio();
        } else if (choice != 0) {
            std::cout << "Nieznana opcja. Sprobuj ponownie.\n";
        }
    }

    std::cout << "Zamykanie programu. Do zobaczenia!\n";
    return 0;
}