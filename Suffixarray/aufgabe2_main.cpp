#include <iostream>
#include <string>
#include <vector>
#include "aufgabe2.hpp"

using namespace std;

int main(int argc, const char* argv[]) {
    // Überprüft die Anzahl der übergebenen Argumente

    // Initialisiert das Suffix-Array und den Vektor für die Treffer
    vector<uint32_t> sa;
    vector<uint32_t> hits;

    // Falls nur ein Argument (der Text) gegeben ist
    if (argc == 2) {
        // Erstellt das Suffix-Array für den gegebenen Text
        construct(sa, argv[1]);

        // Gibt das Suffix-Array Zeile für Zeile aus
        for (const auto& index : sa) {
            cout << index << endl;
        }
        return 0; 
    }
    // Falls Text und ein oder mehrere Suchbegriffe gegeben sind
    else if (argc >= 3) {
        // Erstellt das Suffix-Array für den Text (erstes Argument)
        construct(sa, argv[1]);

        // Durchsucht den Text nach jedem Suchbegriff und gibt die Treffer aus
        for (int i = 2; i < argc; i++) {
            // Sucht nach dem aktuellen Suchbegriff im Text
            find(argv[i], sa, argv[1], hits);

            // Gibt den Suchbegriff und die Treffer aus
            cout << argv[i] << ": ";
            for (const auto& hit : hits) {
                cout << hit << " ";
            }
            cout << endl;
        }
        return 0; 
    }
    // Falls die Eingabe nicht den Erwartungen entspricht
    else {
        cout << "Unerwartete Eingabe\n";
        return -1; // Fehlercode zurückgeben
    }
}
