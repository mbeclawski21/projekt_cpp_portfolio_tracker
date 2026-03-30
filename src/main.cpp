#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "Portfolio.h"
#include "Stock.h"
#include "Crypto.h"

using json = nlohmann::json;

void displayMenu() {
    std::cout << "\n=== PortfolioTracker - Menu ===\n";
    std::cout << "1. Dodaj Akcje (Recznie)\n";
    std::cout << "2. Dodaj Krypto (Z API CoinGecko)\n";
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
        
        // Zabezpieczenie przed wpisaniem litery zamiast cyfry
        if (!(std::cin >> choice)) {
            std::cout << "Nieprawidlowy format wyboru.\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 1) {
            std::string name, symbol, exchange;
            double price;
            std::cout << "Podaj nazwe: "; std::cin >> name;
            std::cout << "Podaj symbol: "; std::cin >> symbol;
            std::cout << "Podaj cene: "; std::cin >> price;
            std::cout << "Podaj gielde: "; std::cin >> exchange;

            myPortfolio.addAsset(std::make_unique<Stock>(name, symbol, price, exchange));
            std::cout << "Akcja dodana!\n";

        } else if (choice == 2) {
            std::string coinId, symbol, network;
            
            std::cout << "Podaj ID krypto (np. bitcoin): ";
            std::cin >> coinId;
            std::cout << "Podaj symbol: ";
            std::cin >> symbol;
            std::cout << "Podaj siec: ";
            std::cin >> network;

            std::cout << "Laczenie z API... Pobieram cene dla: " << coinId << "\n";
            
            std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + coinId + "&vs_currencies=usd";
            cpr::Response r = cpr::Get(cpr::Url{url});

            double livePrice = 0.0;

            if (r.status_code == 200) {
                json data = json::parse(r.text);
                
                if (data.contains(coinId)) {
                    livePrice = data[coinId]["usd"];
                    std::cout << "Pobrano cene: $" << livePrice << "\n";
                } else {
                    std::cout << "Nie znaleziono ceny dla: " << coinId << "\n";
                }
            } else {
                std::cout << "Blad polaczenia (Kod: " << r.status_code << ")\n";
            }

            myPortfolio.addAsset(std::make_unique<Crypto>(coinId, symbol, livePrice, network));
            std::cout << "Krypto dodane do portfela!\n";

        } else if (choice == 3) {
            myPortfolio.displayPortfolio();
        }
    }
    
    std::cout << "Do zobaczenia!\n";
    return 0;
}