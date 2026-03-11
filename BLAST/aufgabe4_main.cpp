#include <iostream>  
#include <string>    
#include <omp.h>     // Inkludiere die OpenMP-Bibliothek für parallele Programmierung

#include "a4_util.h"                
#include "BLAST_Neighborhood.hpp"   

using namespace std; 

int main(int argc, const char* argv[]) {  // Hauptfunktion, argc ist die Anzahl der Argumente, argv sind die Argumente
    // args: 1-SEQ 2-SCORE_MATRIX 3-WORD_SIZE 4-THRESHOLD 5-THREADS

    if (argc != 6) { // Überprüfe, ob genau 5 Argumente übergeben wurden (argc ist immer 1 größer, da das Programm selbst zählt)
        cout << "Korrekter Aufruf: aufgabe4_main <SEQ> <SCORE_MATRIX> <WORD_SIZE> <THRESHOLD> <THREADS>\n";
        return 1;  // Beende das Programm mit einem Fehlercode, wenn die Anzahl der Argumente nicht stimmt
    } else {
        // Lade Scoring-Matrix
        ScoreMatrix scoreMatrix;  // Erstelle ein Objekt der Klasse ScoreMatrix
        scoreMatrix.load(argv[2]);  // Lade die Scoring-Matrix aus der Datei, die im zweiten Argument angegeben ist

        // Generiere Nachbarschaft
        double start, end; // Variablen zur Zeitmessung

        std::vector<NHResult> result;  // Erstelle einen Vektor, um das Ergebnis der Nachbarschaftsberechnung zu speichern
        BLAST_Neighborhood blastNeighborhood;  // Erstelle ein Objekt der Klasse BLAST_Neighborhood
        
        // Zeit messen und Mitgliedsfunktion nutzen, um Nachbarschaft zu generieren
        start = omp_get_wtime();  // Startzeit messen
        // Rufe die Funktion generateNeighborhood auf, um die Nachbarschaft zu berechnen und speichere das Ergebnis in result
        result = blastNeighborhood.generateNeighborhood(argv[1], scoreMatrix, std::stoi(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]));
        end = omp_get_wtime();  // Endzeit messen

        // Ergebnisse ausgeben
        for (NHResult item : result) { // Durchlaufe jedes Element (NHResult) im Ergebnisvektor
            // Infix
            cout << item.infix << ": ";  // Gebe das Infix aus

            // Zugehörige Nachbarn
            for (std::pair<std::string, int> neighbor : item.neighbors)  // Durchlaufe jedes Nachbarpaar (String und Score)
                cout << "(" << neighbor.first << ", " << neighbor.second << ") ";  // Gebe den Nachbarn und seinen Score aus
            cout << endl;  // Neue Zeile nach jeder Infix-Ausgabe
        }

        // Zeit
        cout << "Zeit: " << (end - start) << " Sekunden" << endl;  // Gebe die zur Berechnung benötigte Zeit aus

        return 0; 
    }
}
