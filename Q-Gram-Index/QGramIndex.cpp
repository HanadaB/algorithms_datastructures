#include "QGramIndex.hpp"  
#include "a5_util.hpp"     
#include <vector>          
#include <stdexcept>       
#include <cmath>           
#include <algorithm>      

// Konstruktor der QGramIndex Klasse
QGramIndex::QGramIndex(const std::string& inputText, const uint8_t inputQ)
    : text(inputText), q(inputQ), mask((1 << (2 * q)) - 1) {
    if (q < 1 || q > 13) {
        throw std::invalid_argument("Invalid q!");  // Wirft eine Ausnahme, wenn q ungültig ist
    }
    buildSuffixArray();  // Aufruf der Methode zum Aufbau des Suffix-Arrays
    buildDirectory();    // Aufruf der Methode zum Aufbau des Directories
}

// Methode zum Aufbau des Suffix-Arrays
void QGramIndex::buildSuffixArray() {
    suftab.resize(text.length() - q + 1);  // Passt die Größe des Suffix-Arrays an
    for (uint32_t i = 0; i <= text.length() - q; ++i) {
        suftab[i] = i;  // Initialisiert das Suffix-Array mit Startindizes
    }
    // Sortiert das Suffix-Array basierend auf den Q-Grammen
    std::sort(suftab.begin(), suftab.end(), [&](uint32_t a, uint32_t b) {
        return text.compare(a, q, text, b, q) < 0;
    });
}

// Methode zum Aufbau des Directories
void QGramIndex::buildDirectory() {
    dir.assign(1 << (2 * q), 0);  // Initialisiert das Directory mit 0
    for (uint32_t i = 0; i < suftab.size(); ++i) {
        uint32_t hash = this->hash(text.substr(suftab[i], q));  // Berechnet den Hash-Wert
        dir[hash]++;  // Erhöht den Zähler im Directory
    }
    // Berechnet die kumulative Summe im Directory
    uint32_t sum = 0;
    for (auto& val : dir) {
        sum += val;
        val = sum - val;
    }
}

// Methode zur Berechnung des Hash-Wertes eines Q-Gramms
uint32_t QGramIndex::hash(const std::string& qgram) const {
    if (qgram.length() != q) {
        throw std::invalid_argument("Invalid q-gram. Wrong length!");  // Wirft eine Ausnahme bei falscher Länge
    }
    uint32_t h = 0;
    // Schleife, die jeden Buchstaben des Q-Gramms in einen Hash-Wert umwandelt
    for (char c : qgram) {
        h = (h << 2) | ordValue(c);  // Verschiebt und kombiniert Hash-Werte
    }
    return h;  // Gibt den berechneten Hash-Wert zurück
}

// Methode zur Berechnung des nächsten Hash-Wertes basierend auf dem vorherigen Hash und einem neuen Zeichen
uint32_t QGramIndex::hashNext(const uint32_t prev_hash, const char new_pos) const {
    return ((prev_hash << 2) & mask) | ordValue(new_pos);  // Berechnet den neuen Hash-Wert
}

// Methode zur Rückgabe eines Vektors von Indizes im Text, an denen ein Q-Gramm mit dem Hash-Wert h vorkommt
std::vector<uint32_t> QGramIndex::getHits(const uint32_t h) const {
    if (h >= dir.size()) {
        throw std::invalid_argument("Invalid hash!");  // Wirft eine Ausnahme bei ungültigem Hash-Wert
    }
    std::vector<uint32_t> hits;  // Vektor zur Speicherung der Treffer
    uint32_t start = dir[h];  // Bestimmt den Startindex im Suffix-Array
    uint32_t end = (h + 1 < dir.size()) ? dir[h + 1] : suftab.size();  // Bestimmt den Endindex
    // Schleife, die alle Treffer im angegebenen Bereich hinzufügt
    for (uint32_t i = start; i < end; ++i) {
        hits.push_back(suftab[i]);
    }
    return hits;  // Gibt den Vektor mit Treffern zurück
}

// Methode, um den gespeicherten Text zurückzugeben
const std::string& QGramIndex::getText() const {
    return text;
}

// Methode, um die Q-Gramm-Länge zurückzugeben
uint8_t QGramIndex::getQ() const {
    return q;
}
