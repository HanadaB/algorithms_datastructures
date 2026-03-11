#include "Alignment.hpp" 
#include <vector> 
#include <string> 
#include <algorithm> // Einbinden der Standardbibliothek für Algorithmen wie max
#include <exception> // Einbinden der Standardbibliothek für Ausnahmenbehandlung
#include <stdexcept> // Definition von 'std::runtime_error' enthält

using namespace std; 

// Exception-Klasse für den Fall, dass compute() nicht aufgerufen wurde
class computeexception: public exception {
    // Überschreiben der what()-Methode der Basisklasse exception
    virtual const char* what() const throw() {
        return "compute was not called"; // Rückgabe einer Fehlermeldung
    }
} myex; // Instanz der Exception-Klasse

// Konstruktor: speichert die übergebenen Sequenzen und setzt computed auf false
Alignment::Alignment(const std::string& seq_v, const std::string& seq_h)
    : seq_v(seq_v), seq_h(seq_h), computed(false) {}
// Konstruktor: speichert die übergebenen Sequenzen und setzt computed auf false

// Hauptmethode zur Berechnung des Alignments
void Alignment::compute(const int match, const int mismatch, const int gap, const bool local_align) {
    // Prüfung auf lokale Ausrichtung und Auslösen einer Ausnahme, falls erforderlich
    if (local_align) {
        // Wirft eine Ausnahme, wenn die lokale Ausrichtung angefordert wird,
        // aber nicht unterstützt wird.
        throw runtime_error("Local alignment not supported.");
    }
    
    // Initialisierung der Score- und Traceback-Matrizen
    vector<vector<int>> score_matrix(seq_h.length()+1, vector<int>(seq_v.length()+1, 0));
    vector<vector<int>> traceback_matrix(seq_h.length()+1, vector<int>(seq_v.length()+1, 0));

    // Initialisierung der ersten Zeile und Spalte der Score-Matrix
    for(size_t i = 1; i <= seq_v.length(); i++) { // int muss mit size_t gewechselt werden wegen std::vector und string in 4 for!
        // Initialisiert die erste Zeile mit dem Gap-Score.
        score_matrix[0][i] = i * gap;
    }
    for(size_t j = 1; j <= seq_h.length(); j++) {
        // Initialisiert die erste Spalte mit dem Gap-Score.
        score_matrix[j][0] = j * gap;
    }

    // Ausfüllen der Score-Matrix und Aktualisierung der Traceback-Matrix
    for(size_t y = 1; y <= seq_v.length(); y++) {
        for(size_t x = 1; x <= seq_h.length(); x++) {
            // Berechnung des diagonalen Scores basierend auf Übereinstimmung oder Nichtübereinstimmung
            int diag = (seq_v[y-1] == seq_h[x-1]) ? match : mismatch;
            int score_diag = score_matrix[x-1][y-1] + diag;
            // Berechnung des linken und oberen Scores
            int score_left = score_matrix[x-1][y] + gap;
            int score_up = score_matrix[x][y-1] + gap;

            // Bestimmung des maximalen Scores aus den drei möglichen Bewegungen
            int max_score = max({score_diag, score_left, score_up});
            score_matrix[x][y] = max_score;

            // Aktualisierung der Traceback-Richtung basierend auf dem gewählten maximalen Score
            if (max_score == score_diag) {
                traceback_matrix[x][y] = 2; // diagonal
            } else if (max_score == score_up) {
                traceback_matrix[x][y] = 1; // hoch
            } else {
                traceback_matrix[x][y] = 0; // links
            }
        }
    }

    // Speichern des Endscores nach der Berechnung
    score = score_matrix[seq_h.length()][seq_v.length()];

    // Traceback zur Rekonstruktion des Alignments
    int i = seq_h.length();
    int j = seq_v.length();
    traceback.clear(); // Löscht die bestehende Traceback-Liste
    while(i > 0 && j > 0) {
        // Hinzufügen der Traceback-Richtungen zur Traceback-Liste
        if (traceback_matrix[i][j] == 2) { // diagonal
            traceback.insert(traceback.begin(), 2);
            i--;
            j--;
        } else if (traceback_matrix[i][j] == 1) { // hoch
            traceback.insert(traceback.begin(), 1);
            j--;
        } else { // links
            traceback.insert(traceback.begin(), 0);
            i--;
        }
    }
    // Hinzufügen verbleibender Lücken, falls vorhanden
    while (i > 0) {
        traceback.insert(traceback.begin(), 0);
        i--;
    }
    while (j > 0) {
        traceback.insert(traceback.begin(), 1);
        j--;
    }
    computed = true; // Setzen von computed auf true nach der Berechnung
}

// Gibt den Score des Alignments zurück
int Alignment::getScore() const {
    if (!computed) throw myex; // Überprüfung, ob compute aufgerufen wurde
    return score; // Rückgabe des gespeicherten Scores
}

// Erzeugt die drei Strings für das Alignment: Sequenz 1, Lücken, Sequenz 2
void Alignment::getAlignment(std::string& a1, std::string& gaps, std::string& a2) const {
    if (!computed) throw myex; // Überprüfung, ob compute aufgerufen wurde

    int x = 0, y = 0;
    a1.clear(); // Leeren von a1
    a2.clear(); // Leeren von a2
    gaps.clear(); // Leeren von gaps

// Durchlaufen des Tracebacks und Erzeugen der Alignment-Strings
for(int tb : traceback) {
    if (tb == 2) { // Falls die Traceback-Richtung diagonal ist
        // Fügen Sie das aktuelle Zeichen von seq_v und seq_h hinzu
        a1 += seq_v[y++]; // Füge das nächste Zeichen von seq_v zu a1 hinzu und erhöhe y
        a2 += seq_h[x++]; // Füge das nächste Zeichen von seq_h zu a2 hinzu und erhöhe x
        // Füge ein '|' für Übereinstimmung oder ein Leerzeichen für Nichtübereinstimmung hinzu
        gaps += (a1.back() == a2.back()) ? '|' : ' ';
    } else if (tb == 1) { // Falls die Traceback-Richtung hoch ist
        // Füge das nächste Zeichen von seq_v und ein '-' für seq_h hinzu
        a1 += seq_v[y++]; // Füge das nächste Zeichen von seq_v zu a1 hinzu und erhöhe y
        a2 += '-';        // Füge ein '-' zu a2 hinzu, um eine Lücke darzustellen
        gaps += ' ';      // Füge ein Leerzeichen zu gaps hinzu, um eine Nichtübereinstimmung darzustellen
    } else { // Falls die Traceback-Richtung links ist
        // Füge ein '-' für seq_v und das nächste Zeichen von seq_h hinzu
        a1 += '-';        // Füge ein '-' zu a1 hinzu, um eine Lücke darzustellen
        a2 += seq_h[x++]; // Füge das nächste Zeichen von seq_h zu a2 hinzu und erhöhe x
        gaps += ' ';      // Füge ein Leerzeichen zu gaps hinzu, um eine Nichtübereinstimmung darzustellen
    }
}

}
