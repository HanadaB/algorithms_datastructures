#include <iostream>
#include <string>
#include <sstream>
#include "Alignment.hpp"

using namespace std;

int main(int argc, const char* argv[]) {
    // Überprüfen, ob die Anzahl der Argumente korrekt ist
    if (argc != 3 && argc != 6) {
        cerr << "Fehler: Falsche Anzahl von Argumenten." << endl;
        return 1;
    }

    try {
        // Sequenzen aus den Argumenten lesen
        string seqV = argv[1];
        string seqH = argv[2];

        // Standardwerte für Match, Mismatch und Gap
        int match = 3, mismatch = -1, gap = -2;

        // Wenn zusätzliche Argumente vorhanden sind, diese konvertieren
        if (argc == 6) {
            istringstream(argv[3]) >> match;
            istringstream(argv[4]) >> mismatch;
            istringstream(argv[5]) >> gap;
        }

        // Alignment-Objekt erstellen und berechnen
        Alignment alignment(seqV, seqH);
        alignment.compute(match, mismatch, gap);

        // Ergebnisse abrufen und ausgeben
        string a1, gaps, a2;
        alignment.getAlignment(a1, gaps, a2);
        cout << a1 << endl;
        cout << gaps << endl;
        cout << a2 << endl;
        cout << "score:" << alignment.getScore() << endl;

    } catch (const exception& e) {
        // Fehlerbehandlung für unerwartete Ausnahmen
        cerr << "Ein Fehler ist aufgetreten: " << e.what() << endl;
        return 1;
    }

    return 0;
}
