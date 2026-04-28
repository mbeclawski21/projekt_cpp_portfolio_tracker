#include "ui.h"
#include "imgui.h"
#include "implot.h"
#include "Crypto.h"
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static bool showChartWindow = false;
static std::string activeChartSymbol = "";
static std::string activeChartId = "";
static std::vector<double> chartPrices;
static std::vector<double> chartDays;

static char inputCoinId[64] = "";
static char inputSymbol[16] = "";
static float inputAmount = 1.0f;
static float inputPurchasePrice = 0.0f;

static float refreshTimer = 0.0f;

static int assetToDeleteIndex = -1;
static int assetToEditIndex = -1;
static bool openEditPopup = false;
static float editAmount = 0.0f;
static float editPurchasePrice = 0.0f;

std::string ToLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
    s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
    return s;
}

void SavePortfolio(const Portfolio& p) {
    json j = json::array();
    for (const auto& a : p.getAssets()) {
        j.push_back({
            {"id", a->getName()},
            {"symbol", a->getSymbol()},
            {"amount", a->getAmount()},
            {"purchasePrice", a->getPurchasePrice()},
            {"currentPrice", a->getPrice()}
        });
    }
    std::ofstream file("portfolio.json");
    file << j.dump(4);
}

void LoadPortfolio(Portfolio& p) {
    std::ifstream file("portfolio.json");
    if (!file.is_open()) return;
    json j;
    file >> j;
    for (const auto& item : j) {
        p.addAsset(std::make_unique<Crypto>(
            item["id"], item["symbol"], item["currentPrice"], item["amount"], item["purchasePrice"]
        ));
    }
}

void RefreshAllPrices(Portfolio& myPortfolio) {
    auto& assets = myPortfolio.getAssets();
    if (assets.empty()) return;

    std::string ids = "";
    for (size_t i = 0; i < assets.size(); ++i) {
        ids += assets[i]->getName();
        if (i < assets.size() - 1) ids += ",";
    }

    std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + ids + "&vs_currencies=usd";
    cpr::Response r = cpr::Get(cpr::Url{url});

    if (r.status_code == 200) {
        json data = json::parse(r.text);
        for (const auto& asset : assets) {
            std::string id = asset->getName();
            if (data.contains(id)) {
                asset->updatePrice(data[id]["usd"]);
            }
        }
    }
}

void FetchHistoricalData(std::string coinId) {
    chartPrices.clear();
    chartDays.clear();
    std::string cleanId = ToLower(coinId);
    std::string url = "https://api.coingecko.com/api/v3/coins/" + cleanId + "/market_chart?vs_currency=usd&days=7&interval=daily";
    cpr::Response r = cpr::Get(cpr::Url{url});
    if (r.status_code == 200) {
        json data = json::parse(r.text);
        if (data.contains("prices")) {
            int dayCounter = 1;
            for (auto& entry : data["prices"]) {
                chartPrices.push_back(entry[1].get<double>());
                chartDays.push_back((double)dayCounter++);
            }
        }
    }
}

void RenderUI(Portfolio& myPortfolio) {
    static bool firstLoad = true;
    if (firstLoad) {
        LoadPortfolio(myPortfolio);
        firstLoad = false;
    }

    refreshTimer += ImGui::GetIO().DeltaTime;
    if (refreshTimer > 300.0f) {
        RefreshAllPrices(myPortfolio);
        refreshTimer = 0.0f;
    }

    ImGui::SetNextWindowPos(ImVec2(410, 20), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(850, 300), ImGuiCond_Always);
    ImGuiWindowFlags portfolioFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Moj Portfel Inwestycyjny", nullptr, portfolioFlags);
    
    double totalVal = myPortfolio.calculateTotalValue();
    double totalCost = 0;
    for (const auto& a : myPortfolio.getAssets()) {
        totalCost += a->getTotalCost();
    }
    double totalProfit = totalVal - totalCost;
    double totalProfitPct = (totalCost > 0) ? (totalProfit / totalCost) * 100.0 : 0.0;
    
    ImVec4 totalPlColor = (totalProfit >= 0) ? ImVec4(0,1,0,1) : ImVec4(1,0,0,1);

    ImGui::Text("Calkowita wartosc: $%.2f", totalVal);
    ImGui::SameLine();
    ImGui::Text(" | Calkowity zysk: ");
    ImGui::SameLine();
    ImGui::TextColored(totalPlColor, "$%.2f (%.2f%%)", totalProfit, totalProfitPct);
    
    ImGui::SameLine(ImGui::GetWindowWidth() - 160.0f);
    if (ImGui::Button("Synchronizuj kursy", ImVec2(140.0f, 0))) {
        RefreshAllPrices(myPortfolio);
        refreshTimer = 0.0f;
    }

    ImGui::Separator();

    if (ImGui::BeginTable("TabelaAktywow", 8, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Symbol");
        ImGui::TableSetupColumn("Ilosc");
        ImGui::TableSetupColumn("Cena Kupna");
        ImGui::TableSetupColumn("Cena Aktualna");
        ImGui::TableSetupColumn("Wartosc");
        ImGui::TableSetupColumn("Zysk $");
        ImGui::TableSetupColumn("Zysk %");
        ImGui::TableSetupColumn("Akcje");
        ImGui::TableHeadersRow();

        int n = 0;
        for (const auto& asset : myPortfolio.getAssets()) {
            double pl = asset->getProfitLoss();
            ImVec4 plColor = (pl >= 0) ? ImVec4(0,1,0,1) : ImVec4(1,0,0,1);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", asset->getSymbol().c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%.4f", asset->getAmount());
            ImGui::TableSetColumnIndex(2); ImGui::Text("$%.2f", asset->getPurchasePrice());
            ImGui::TableSetColumnIndex(3); ImGui::Text("$%.2f", asset->getPrice());
            ImGui::TableSetColumnIndex(4); ImGui::Text("$%.2f", asset->getTotalValue());
            ImGui::TableSetColumnIndex(5); ImGui::TextColored(plColor, "$%.2f", pl);
            ImGui::TableSetColumnIndex(6); ImGui::TextColored(plColor, "%.2f%%", asset->getProfitLossPercentage());
            
            ImGui::TableSetColumnIndex(7);
            
            std::string bl = "Wykres##" + std::to_string(n);
            if (ImGui::Button(bl.c_str())) {
                showChartWindow = true;
                activeChartSymbol = asset->getSymbol();
                activeChartId = asset->getName();
                FetchHistoricalData(activeChartId);
            }

            ImGui::SameLine();
            std::string btnEdit = "Edytuj##" + std::to_string(n);
            if (ImGui::Button(btnEdit.c_str())) {
                assetToEditIndex = n;
                editAmount = asset->getAmount();
                editPurchasePrice = asset->getPurchasePrice();
                openEditPopup = true;
            }

            ImGui::SameLine();
            std::string btnDel = "Usun##" + std::to_string(n);
            if (ImGui::Button(btnDel.c_str())) {
                assetToDeleteIndex = n;
            }
            n++;
        }
        ImGui::EndTable();
    }
    ImGui::End();

    if (assetToDeleteIndex >= 0) {
        myPortfolio.removeAssetByIndex(assetToDeleteIndex);
        SavePortfolio(myPortfolio);
        assetToDeleteIndex = -1;
    }

    if (openEditPopup) {
        ImGui::OpenPopup("Edytuj Aktywo");
        openEditPopup = false;
    }

    if (ImGui::BeginPopupModal("Edytuj Aktywo", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputFloat("Nowa Ilosc", &editAmount);
        ImGui::InputFloat("Nowa Cena Kupna ($)", &editPurchasePrice);

        if (ImGui::Button("Zapisz", ImVec2(120, 0))) {
            if (assetToEditIndex >= 0) {
                myPortfolio.updateAsset(assetToEditIndex, editAmount, editPurchasePrice);
                SavePortfolio(myPortfolio);
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Anuluj", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(380, 300), ImGuiCond_Always);
    ImGuiWindowFlags addFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Dodaj Krypto (CoinGecko API)", nullptr, addFlags);
    
    ImGui::PushItemWidth(130.0f);

    ImGui::InputText("ID (np. bitcoin)", inputCoinId, IM_ARRAYSIZE(inputCoinId));
    ImGui::SameLine();
    if (ImGui::Button("Lista", ImVec2(60, 0))) {
        ImGui::OpenPopup("ListaKryptoPopup");
    }

    if (ImGui::BeginPopup("ListaKryptoPopup")) {
        ImGui::Text("Popularne waluty:");
        ImGui::Separator();

        ImGui::BeginChild("KryptoChild", ImVec2(180, 150), true);
        
        struct Preset { const char* id; const char* symbol; const char* name; };
        static Preset cryptos[] = {
            {"bitcoin", "BTC", "Bitcoin"}, {"ethereum", "ETH", "Ethereum"},
            {"tether", "USDT", "Tether"}, {"binancecoin", "BNB", "BNB"},
            {"solana", "SOL", "Solana"}, {"ripple", "XRP", "XRP"},
            {"usd-coin", "USDC", "USDC"}, {"cardano", "ADA", "Cardano"},
            {"dogecoin", "DOGE", "Dogecoin"}, {"chainlink", "LINK", "Chainlink"},
            {"polkadot", "DOT", "Polkadot"}, {"litecoin", "LTC", "Litecoin"},
            {"matic-network", "MATIC", "Polygon"}
        };

        for (const auto& c : cryptos) {
            if (ImGui::Selectable(c.name)) {
                snprintf(inputCoinId, sizeof(inputCoinId), "%s", c.id);
                snprintf(inputSymbol, sizeof(inputSymbol), "%s", c.symbol);
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndChild();
        ImGui::EndPopup();
    }

    ImGui::InputText("Symbol (np. BTC)", inputSymbol, IM_ARRAYSIZE(inputSymbol));
    ImGui::InputFloat("Ilosc", &inputAmount);
    ImGui::InputFloat("Cena zakupu ($)", &inputPurchasePrice);
    ImGui::PopItemWidth();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    if (ImGui::Button("Pobierz cene i dodaj", ImVec2(-1, 0))) {
        if (strlen(inputCoinId) > 0 && strlen(inputSymbol) > 0) {
            std::string coinId = ToLower(std::string(inputCoinId));
            std::string symbol = std::string(inputSymbol);
            std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + coinId + "&vs_currencies=usd";
            cpr::Response r = cpr::Get(cpr::Url{url});
            if (r.status_code == 200) {
                json data = json::parse(r.text);
                if (data.contains(coinId)) {
                    double livePrice = data[coinId]["usd"];
                    myPortfolio.addAsset(std::make_unique<Crypto>(coinId, symbol, livePrice, (double)inputAmount, (double)inputPurchasePrice));
                    SavePortfolio(myPortfolio);
                    inputCoinId[0] = '\0';
                    inputSymbol[0] = '\0';
                    inputAmount = 1.0f;
                    inputPurchasePrice = 0.0f;
                }
            }
        }
    }
    ImGui::End();

    if (showChartWindow) {
        ImGui::SetNextWindowPos(ImVec2(410, 340), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(850, 360), ImGuiCond_Always);
        ImGuiWindowFlags chartFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

        if (ImGui::Begin(("Historia - " + activeChartSymbol).c_str(), &showChartWindow, chartFlags)) {
            if (!chartPrices.empty()) {
                if (ImPlot::BeginPlot("Kurs 7d", ImVec2(-1, -40))) {
                    ImPlot::PlotLine(activeChartSymbol.c_str(), chartDays.data(), chartPrices.data(), (int)chartPrices.size());
                    ImPlot::EndPlot();
                }
                ImGui::Separator();
                auto min_it = std::min_element(chartPrices.begin(), chartPrices.end());
                auto max_it = std::max_element(chartPrices.begin(), chartPrices.end());
                ImGui::TextColored(ImVec4(1, 0.5f, 0.5f, 1), "Cena MIN (7d): $%.2f", *min_it);
                ImGui::SameLine(300);
                ImGui::TextColored(ImVec4(0.5f, 1, 0.5f, 1), "Cena MAX (7d): $%.2f", *max_it);
            }
        }
        ImGui::End();
    }
}