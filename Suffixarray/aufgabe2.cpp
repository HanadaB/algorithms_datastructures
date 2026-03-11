#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include "aufgabe2.hpp"

using namespace std;

// Funktion zum Erstellen des Suffix-Arrays
void construct(vector<uint32_t>& sa, const string& text) {
    sa.clear(); // Löscht das existierende Suffix-Array
    sa.resize(text.size()); // Setzt die Größe des Suffix-Arrays entsprechend der Textlänge
    for (size_t i = 0; i < text.size(); ++i) {
        sa[i] = i; // Füllt das Suffix-Array mit den Startindizes der Suffixe
    }

    // Sortiert das Suffix-Array basierend auf den Suffixen
    sort(sa.begin(), sa.end(), [&text](uint32_t a, uint32_t b) {
    // Vergleicht die Suffixe, die bei den Indizes a und b beginnen, mithilfe von strcmp.
    // strcmp vergleicht die beiden Suffixe und gibt einen Wert kleiner als 0 zurück,
    // wenn das erste Suffix (bei a) lexikografisch kleiner ist als das zweite (bei b).
        return strcmp(text.c_str() + a, text.c_str() + b) < 0; 
    });
}

// Hilfsfunktion zur Bestimmung der linken Grenze in der binären Suche
int findLp(const string& query, const vector<uint32_t>& sa, const string& text) {
    int l = 0, r = sa.size(); // Start- und Endpunkt der Suche
    while (l < r) {
        int mid = l + (r - l) / 2; // Mittelpunkt der aktuellen Suche
        // Vergleicht den Mittelpunkt mit der Abfrage
        if (text.compare(sa[mid], query.size(), query) < 0) {
            l = mid + 1; // Verschiebt den Startpunkt, wenn das Mittelstück kleiner ist
        } else {
            r = mid; // Verschiebt den Endpunkt, wenn das Mittelstück größer oder gleich ist
        }
    }
    return l; // Gibt die linke Grenze zurück
}

// Hilfsfunktion zur Bestimmung der rechten Grenze in der binären Suche
int findRp(const string& query, const vector<uint32_t>& sa, const string& text) {
    int l = 0, r = sa.size(); // Start- und Endpunkt der Suche
    while (l < r) {
        int mid = l + (r - l) / 2; // Mittelpunkt der aktuellen Suche
        // Vergleicht den Mittelpunkt mit der Abfrage
        if (text.compare(sa[mid], query.size(), query) <= 0) {
            l = mid + 1; // Verschiebt den Startpunkt, wenn das Mittelstück kleiner oder gleich ist
        } else {
            r = mid; // Verschiebt den Endpunkt, wenn das Mittelstück größer ist
        }
    }
    return r; // Gibt die rechte Grenze zurück
}

// Funktion zur Suche einer Abfragezeichenfolge im Text mit einem Suffix-Array
void find(const string& query, const vector<uint32_t>& sa, const string& text, vector<uint32_t>& hits) {
    hits.clear(); // Löscht vorherige Suchergebnisse
    if (query.empty()) return; // Beendet, wenn die Abfrage leer ist

    int Lp = findLp(query, sa, text); // Bestimmt die linke Grenze der Übereinstimmung
    int Rp = findRp(query, sa, text); // Bestimmt die rechte Grenze der Übereinstimmung

    for (int i = Lp; i < Rp; i++) {
        hits.push_back(sa[i]); // Fügt alle Übereinstimmungen zwischen Lp und Rp zum Ergebnis hinzu
    }

    sort(hits.begin(), hits.end()); // Sortiert die Trefferindizes in aufsteigender Reihenfolge
}

