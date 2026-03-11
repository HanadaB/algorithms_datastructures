#include "ACTrie.hpp"  
#include <iostream>   
#include <queue>     
#include <stdexcept>  

// Konstruktor der ACTrie-Klasse
ACTrie::ACTrie(const std::vector<std::string>& needles) {
    if (needles.empty()) {
        throw std::logic_error("At least one needle is required");
    }

    // Fügt den Wurzelknoten in den Vektor von Knoten (ACVector) ein
    ACVector.emplace_back();

    // Durchläuft alle Needlen 
    for (size_t i = 0; i < needles.size(); ++i) {
        // Erstellt eine Referenz auf das aktuelle Needle-String
        const std::string& needle = needles[i];

        // Startet bei der Wurzel des Tries (Index 0)
        int current_node = 0;  
        // Durchläuft jedes Zeichen des aktuellen Needle-Strings
        for (char ch : needle) {
            if (ch < 'A' || ch > 'Z') {
                throw std::exception();
            }

            // Überprüft, ob das Zeichen noch nicht im Trie vorhanden ist
            if (ACVector[current_node].children.find(ch) == ACVector[current_node].children.end()) {
                // Fügt ein neues Kind mit dem aktuellen Zeichen hinzu und 
                ACVector[current_node].children[ch] = ACVector.size();
                ACVector.emplace_back(); //erstellt einen neuen Knoten im Trie
            }
            // Aktualisiert den aktuellen Knoten zum Kindknoten
            current_node = ACVector[current_node].children[ch];
        }
        // Fügt den Index des aktuellen Needle-Strings in die Liste der Needleindizes des Knotens ein
        ACVector[current_node].needle_indices.push_back(i);
    }

    // Initialisiert die Queue für die Breitensuche (BFS)
    std::queue<int> q;
    // Durchläuft die direkte Kinder des Wurzelknotens
    for (auto& [ch, child_idx] : ACVector[0].children) {
        // Setzt den Suffix-Link des Kindknotens auf die Wurzel (Index 0)
        ACVector[child_idx].suffix_link = 0;
        // Fügt den Index des Kindknotens in die Queue ein
        q.push(child_idx);
    }

    // Führt die Breitensuche (BFS) durch
    while (!q.empty()) {
        // Entnimmt den vordersten Knoten aus der Queue
        int node = q.front();
        q.pop(); // Entfernt den Knoten aus der Queue

        // Durchläuft die Kinder des aktuellen Knotens
        for (auto& [ch, child_idx] : ACVector[node].children) {
            // Fügt das Kind in die Queue ein(child_idx ist index von Kindknoten im Trie)
            q.push(child_idx);

            // Initialisiere nbm mit dem Suffix-Link vom aktuellen Knoten
            int nextBestMatch = ACVector[node].suffix_link;
            
            //solange nbm nicht auf Wurzel zeigt und keine Kinkonten mit dem aktuellen Zeichen gibt
            while (nextBestMatch != 0 && ACVector[nextBestMatch].children.find(ch) == ACVector[nextBestMatch].children.end()) {
                nextBestMatch = ACVector[nextBestMatch].suffix_link; //aktualisiere nbm mit dem suffix-Link vom Knoten,auf dem nbm gerade zeigt 
            }

            // wenn so einen direkten Übergang ex. bzw. Kindknoten mit dem aktuelle Zeichen innerhalb von Knoten ex.
            if (ACVector[nextBestMatch].children.find(ch) != ACVector[nextBestMatch].children.end()) {
                nextBestMatch = ACVector[nextBestMatch].children[ch];// aktualisiere nbm auf den Index dieses Kinderknoten, welche gerade nbm darauf zeigt
            }

            // Setze den Suffix-Link des kindknotens auf längsten Suffix, auf dem nbm zeigt
            ACVector[child_idx].suffix_link = nextBestMatch;

            // Diese Schleife kopiert alle Needleindizes von Knoten ,die nbm repräsentiert zum aktuellen Kindknoten child_idx
            for (int idx : ACVector[nextBestMatch].needle_indices) {
                ACVector[child_idx].needle_indices.push_back(idx);
            }
        }
    }

    // Variablen für Suchfunktion erstellen und zurücksetzen
    this->current_search_index = 0; // Initialisiert den Suchindex für den Trie.
    this->query_pos = 0;            // Setzt die Position innerhalb der Query zurück.
    this->needleslist = needles;    // Speichert die Liste der needles.

}

// Setzt die zu durchsuchende haystack als Instanz für ACTrie
void ACTrie::setQuery(const std::string& haystack) {
    this->query = haystack;         // Setzt den neuen Query.
    this->current_search_index = 0; // Bereitet den Suchindex für den neuen Suchtext vor.
    this->query_pos = 0;            // Beginnt die Suche im neuen Text von vorne.

}

//Methode next sucht nach Vorkommen von Needles in haystack
bool ACTrie::next(std::vector<Hit>& hits) {
    hits.clear(); // Bereitet das `hits`-Vektor für neue Ergebnisse vor.

    while (this->query_pos < query.length()) { // Durchläuft die Query.
        char ch = query[this->query_pos]; // Liest das aktuelle Zeichen.

        // Sucht rückwärts nach dem längsten passenden Suffix, wenn kein direkter Übergang existiert.
        while (current_search_index != 0 && ACVector[current_search_index].children.find(ch) == ACVector[current_search_index].children.end()) {
            current_search_index = ACVector[current_search_index].suffix_link; // Folgt die Suche dem Suffix-Link zum nbm.
        }

        // Wechselt zum Kindknoten, wenn ein direkter Übergang für `ch` existiert.
        if (ACVector[current_search_index].children.find(ch) != ACVector[current_search_index].children.end()) {
            current_search_index = ACVector[current_search_index].children[ch]; //aktualisiert Suchindex zum Index des Kindknoten vom aktuellen Zeichen
        }

        // Prüft, ob der aktuelle Knoten Needleindizes enthält.
        if (!ACVector[current_search_index].needle_indices.empty()) {
            for (int needle_idx : ACVector[current_search_index].needle_indices) { // Für jedes gefundene Needle...
                size_t pattern_length = needleslist[needle_idx].length();
                size_t hit_position = this->query_pos - pattern_length + 1; // Berechnet die Position des Hits.
                hits.emplace_back(needle_idx, hit_position); // Speichert den Hits.
            }
            this->query_pos++; 
            return true; 
        }
        this->query_pos++; 
    }
    return false; 
}

//Methode für textuelle Repräsentation von Trie
std::string ACTrie::getTree() const {
    // Kombiniert das Zeichen '0' als Repräsentation des Wurzelknotens mit dem Ergebnis der rekursiven Baumdarstellung,
    // die bei Knoten 0 (der Wurzel) beginnt.
    return ('0') + tree_recursion(0); 
}


std::string ACTrie::tree_recursion(int index) const {
    if (index >= ACVector.size()) {
        throw std::logic_error("Index out of bounds in tree_recursion");
    }

    std::string tree = ""; // Initialisiert den String, der den Teilbaum darstellt.

    // Durchläuft jedes Kind des aktuellen Knotens, identifiziert durch `index`.
    for (const auto& child : ACVector[index].children) {
        // Fügt für jedes Kind eine Klammer hinzu, die das Zeichen (child.first) und 
        // die rekursive Darstellung des Kindknotens (child.second) enthält.
        tree += "(" + std::string(1, child.first) + tree_recursion(child.second) + ")";
    }
    return tree; // Gibt die String-Darstellung des Teilbaums zurück.
}