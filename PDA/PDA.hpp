#include <stack>
#pragma once

class PDA {
public:
    enum class Language {
        HAIRPIN, // Akzeptiert RNA-Hairpins
        BRACKETS // Zusatzaufgabe
    };

    enum class State {
        IN_PROGRESS = 0, // Sequenz ist bisher gültig, aber nicht vollständig
        SUCCESS = 1,     // Aktuell im akzeptierenden Zustand, z.B. Stack ist leer nach Lesen des finalen $ Symbols
        FAIL = 2         // Sequenz ist nicht in der Sprache
    };

    PDA(Language l = Language::HAIRPIN); // Konstruktor spezifiziert die zu prüfende Sprache (HAIRPIN als Standard)
    
    State next(char a); // Liest das nächste Symbol und durchläuft Zustände
    
    void reset(); // Setzt den Automaten zurück in den Anfangszustand
    
    
protected:
    std::stack<char> stack; // PDA-Stack
    State state;            // Aktueller Zustand des PDA
    int position; // Hinzugefügt, um die aktuelle Position in der Sequenz zu verfolgen
    Language language;      // Sprache des PDA
   
    bool isBase(char c);    // Hilfsfunktion, um zu überprüfen, ob das Zeichen eine Base ist
    char complement(char c); // Hilfsfunktion, um das komplementäre Zeichen zu erhalten
    bool isValidHairpinLength(int pos); // Überprüft die Länge der Hairpin-Sequenz
};
