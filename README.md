# Portfolio Tracker
## Opis Projektu
Portfolio tracker to aplikacja służąca do śledzenia i zarządzania portfelem inwestycyjnym. System pozwala na monitorowanie różnych aktywów, wyliczenie całkowitej wartości portfela, zysków oraz strat.

## Tydzien 1 - Wprowadzanie danych: Użytkownik może ręcznie dodać do portfela dwa rodzaje aktywów: Akcje (Stock) oraz Kryptowaluty (Crypto). 
* Rozróżnienie typów: System rozróżnia dane specyficzne dla typu - dla akcji podajemy nazwę giełdy, dla krypto nazwę sieci (np. ERC20).
* Możliwość wyświetlenia listy wszystkich posiadanych walorów w formie czytelnego raportu w konsoli.
* Interfejs sterowania: Implementacja menu tekstowego pozwalającego na nawigację między funkcjami (Dodaj/Pokaż/Wyjdź).
* Program pozwala na dodawanie dowolnej liczby obiektów w trakcie działania
* Integracja z Live API (CoinGecko): Program automatycznie pobiera aktualne kursy kryptowalut z internetu. Użytkownik nie musi już ręcznie wpisywać cen.

## Tydzien 3 - Migracja do GUI
* **Nowy silnik graficzny:** Porzucenie standardowego wejścia/wyjścia konsolowego (`std::cin`/`std::cout`) na rzecz biblioteki **Dear ImGui**. Aplikacja wykorzystuje teraz nowoczesny interfejs oparty na oknach i interaktywnych tabelach.
* **Wizualizacja danych:** Integracja biblioteki **ImPlot**, umożliwiającej rysowanie dynamicznych wykresów cen aktywów w osobnych oknach.
* **Obsługa polskich znaków:** Implementacja wsparcia dla czcionek systemowych (*Segoe UI*) z zakresem *Latin Extended*.
* **Automatyzacja budowania (CMake):** Zastosowanie modułu `FetchContent` do automatycznego pobierania i konfigurowania zależności (GLFW, ImGui, ImPlot, CPR, JSON) bezpośrednio z repozytoriów zdalnych.
* **Architektura kodu:** Rozdzielenie warstwy logicznej od prezentacji (UI) oraz przejście na inteligentne wskaźniki (`std::unique_ptr`) dla bezpieczniejszego zarządzania pamięcią.
* **Komunikacja z API:** Wykorzystanie biblioteki **CPR** do pobierania kursów kryptowalut w czasie rzeczywistym, co uniezależnia aplikację od danych wpisywanych "na sztywno".