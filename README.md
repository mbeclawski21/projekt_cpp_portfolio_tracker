# Portfolio Tracker
## Opis Projektu
Portfolio tracker to aplikacja służąca do śledzenia i zarządzania portfelem inwestycyjnym. System pozwala na monitorowanie różnych aktywów, wyliczenie całkowitej wartości portfela, zysków oraz strat.

## Tydzien 1 - Wprowadzanie danych: Użytkownik może ręcznie dodać do portfela dwa rodzaje aktywów: Akcje (Stock) oraz Kryptowaluty (Crypto). 
* Rozróżnienie typów: System rozróżnia dane specyficzne dla typu - dla akcji podajemy nazwę giełdy, dla krypto nazwę sieci (np. ERC20).
* Możliwość wyświetlenia listy wszystkich posiadanych walorów w formie czytelnego raportu w konsoli.
* Interfejs sterowania: Implementacja menu tekstowego pozwalającego na nawigację między funkcjami (Dodaj/Pokaż/Wyjdź).
* Program pozwala na dodawanie dowolnej liczby obiektów w trakcie działania
* Integracja z Live API (CoinGecko): Program automatycznie pobiera aktualne kursy kryptowalut z internetu. Użytkownik nie musi już ręcznie wpisywać cen.

## Tydzień 3 - Migracja do GUI i Rozbudowa Logiki Portfela

* **Nowy silnik graficzny:** Porzucenie standardowego wejścia/wyjścia konsolowego (`std::cin`/`std::cout`) na rzecz biblioteki **Dear ImGui**. Aplikacja wykorzystuje nowoczesny interfejs okienkowy.
* **Wizualizacja danych:** Integracja biblioteki **ImPlot**, umożliwiającej rysowanie wykresów cen aktywów.
* **Integracja z API:** Wykorzystanie biblioteki **CPR** do pobierania kursów kryptowalut w czasie rzeczywistym z CoinGecko.
* **Obsługa ilości (Amount):** Dodanie funkcjonalności śledzenia liczby posiadanych jednostek danego aktywa. System automatycznie oblicza wartość każdego elementu oraz sumaryczną wartość całego portfela (Cena * Ilosc).
* **Refaktoryzacja i czystość kodu:** * Usunięcie zbędnych pól (np. "Sieć"), które nie wpływały na wycenę aktywów.
    * Przejście na inteligentne wskaźniki (`std::unique_ptr`) w zarządzaniu obiektami portfela.
    * Optymalizacja interfejsu pod kątem czytelności i kompatybilności (rezygnacja z polskich znaków na rzecz standardowego kodowania ASCII/UTF-8).
* **Automatyzacja CMake:** Pełna konfiguracja `CMakeLists.txt` z modułem `FetchContent`, co pozwala na automatyczne pobieranie wszystkich bibliotek przy pierwszym budowaniu projektu.