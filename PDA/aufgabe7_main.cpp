#include <iostream>
#include "PDA.hpp"
#include <algorithm>

using namespace std;

int main(int argc, const char* argv[]){
    // Überprüft, ob die richtige Anzahl von Argumenten übergeben wurde
    if (argc != 2) {
        cout << "Korrekte Nutzung: aufgabe7_main <RNA-Sequence>\n";
        return -1;
    }

    std::string RNA_seq = std::string(argv[1]);
    std::reverse(RNA_seq.begin(), RNA_seq.end()); // Sequenz umkehren
    RNA_seq += '$'; // Fügt das Endsymbol hinzu

    // Instanziiert einen neuen PDA für die Hairpin-Erkennung
    PDA pda = PDA(PDA::Language::HAIRPIN); // Hairpin als Standard

    // Überprüft die RNA-Sequenz auf nicht erwartete Symbole
    for (char current : RNA_seq) {
        if (current != 'a' && current != 'c' && current != 'g' && current != 'u' && current != '$') {
            cout << "Unerwartetes Symbol in der eingegebenen Sequenz\n";
            return -2;
        }

        // Füttert das aktuelle Symbol in den PDA und überprüft den Zustand
        PDA::State state = pda.next(current);
        if (state == PDA::State::FAIL) {
            cout << "FAIL\n";
            return 1; // Rückgabewert 1 bei Versagen
        }
        if (state == PDA::State::SUCCESS) {
            cout << "ACCEPT\n";
            return 0; // Rückgabewert 0 bei Erfolg
        }
        // Wenn IN_PROGRESS, fahren Sie mit der nächsten Iteration fort
    }

    // Keine Ausgabe für FAIL am Ende nötig, da der Zustand bereits innerhalb der Schleife geprüft wird
    // Das Endsymbol '$' sorgt dafür, dass der letzte Zustand geprüft wird
    return 0;
}
