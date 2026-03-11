#include "PDA.hpp"
#include <iostream>
#include <algorithm>
using namespace std;

PDA::PDA(Language l) : state(State::IN_PROGRESS), position(0) {
    // Initialisierung und Einstellung der maxMismatches
}

bool PDA::isBase(char c) {
    return c == 'a' || c == 'c' || c == 'g' || c == 'u';
}

char PDA::complement(char c) {
    switch (c) {
        case 'a': return 'u';
        case 'u': return 'a';
        case 'c': return 'g';
        case 'g': return 'c';
        default:  return '\0';
    }
}

PDA::State PDA::next(char a) {
    if (state == State::FAIL) return state;

    if (!isBase(a) && a != '$') {
        state = State::FAIL;
        return state;
    }

    if (a == '$') {
        // Überprüfen, ob der Stack leer ist und ob die Position eine gültige Hairpin-Länge hat
        state = (stack.empty() && isValidHairpinLength(position)) ? State::SUCCESS : State::FAIL;
        return state;
    }

    

    position++; // Aktualisieren der Position bei jedem Zeichen
    return State::IN_PROGRESS;
}

bool PDA::isValidHairpinLength(int pos) {
    const int MIN_HAIRPIN_LENGTH = 10; // Setzen Sie hier den gewünschten Mindestwert
    return pos >= MIN_HAIRPIN_LENGTH;
}

void PDA::reset() {
    while (!stack.empty()) {
        stack.pop();
    }
    state = State::IN_PROGRESS;
}
