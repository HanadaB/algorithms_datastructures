#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cstring>
#include "aufgabe1.h"

using namespace std;


void Horspool::setPattern(const std::string& pat){
	
	pattern = pat; //Setzt das Muster, das später gesucht werden.
	patt_leng = pat.length(); //Speichert die Länge  des Pattern.
	
    for(int i = 0; i < 128; i++){ // Look Up Tabelle wird mit der Länge des Pattern initialisiert.
		lookupTable[i] = patt_leng;
	}
	
	for(size_t j = 0; j < patt_leng - 1; j++ ){
		lookupTable[static_cast<int>(pat[j])] = patt_leng - 1 -j;	
	} // Die Lookup Tabelle wird mit safe shift für jedes Zeichen im Pattern aktualisiert.			
}

const std::string& Horspool::getPattern() const {
	return pattern; // Gibt das gesetzte Pattern zurück.
}	
	 
std::vector<size_t> Horspool::getHits(const std::string& text) const {
	
	std::vector<size_t>indices; //Erstell einen leeren Vektor,um indices von matches da zu speichern.
	int pos = -1; //Start pos ist -1, damit pos + j beim ersten Durchlauf 0 ist.
	size_t t_leng = text.length();
	
	
	// Solange die Position + die Länge des Patterns kliener als die Textlänge ist, 
	//wird der Text durchlaufen.
	while(pos < ((static_cast<int>(t_leng)) - (static_cast<int>(patt_leng)))){
		int j = patt_leng; // Setzt j auf die Länge des Patterns , um rückwärts durch das Pattern zu gehen.
		alignCheck_(pos + 1); // Die Ausrichtung wird überprüft.
		
		//Die Zeichen des Textes und des Pattern werden rückwärts überprüft, solange sie übereinstimmen.
		while((j > 0) && (text[pos + j] == pattern[j - 1])) {
			j--;
		}
		
		//Wenn j o erreicht, heißt das, dass ein match gefunden wurde.
		if (j == 0) indices.push_back(pos + 1);
		// Basierend auf dem letzten Zeichen vom aktuellen Abschnitt wird für die nächste Suche shift berechnet.
		pos += getShift_(text[pos + patt_leng]);
	}
	
	return indices;
}

uint32_t Horspool::getShift_(const char last_char) const {
	
	return lookupTable[static_cast<int>(last_char)]; // Gibt safe shift für das angegebene Zeichen zurück.
}
  
